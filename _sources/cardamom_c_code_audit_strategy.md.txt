# CARDAMOM C Code: Robustness and Diagnostic Audit Strategy

**Objective:** To systematically audit the CARDAMOM C codebase to ensure cross-platform stability, identify hidden memory leaks or undefined behaviors, and optimize runtime performance.

This strategy is broken into three escalating tiers, moving from static text analysis to active runtime profiling, utilizing CARDAMOM's existing test files and compilation scripts.

**⚠️ IMPORTANT: Follow the tiers IN ORDER.** Tier 1 static analysis catches bugs (like uninitialized variables) that runtime sanitizers CANNOT detect. Jumping to Tier 2/3 will miss critical issues.

## Tier 1: Static Analysis & Code Review (Text-Based)

*The goal here is to analyze the source code without executing it, looking for structural issues, logical flaws, and platform-dependent risks. Because CARDAMOM relies heavily on interdependent files and user-defined structs, we will evaluate the codebase holistically.*

### 1. AI-Assisted Whole-Codebase Review

Rather than reviewing isolated functions, we will leverage large-context AI models (like Claude) to map the entire project architecture.

* **Preparation:** 
  * Provide the AI with `C/LLM_DEPENDENCIES.txt`. This compile-time printout acts as a critical roadmap, allowing Claude to easily navigate the complex interdependencies of the `.c` files.
  * Next, supply the actual code. Ensure header files (which contain the crucial `struct` definitions) are processed first so the AI learns the data architecture before analyzing the execution logic in the `.c` files.

* **Prompting Strategy:**
  * *"Here is `C/LLM_DEPENDENCIES.txt` to map the codebase, followed by the complete C codebase for CARDAMOM. First, review the header files to understand the core structs and data models. Then, audit the entire codebase for cross-file variable definition ambiguities, shadowing, and potential `malloc`/`free` mismatches across different modules."*
  * *"Analyze the global inter-dependencies based on the dependency tree provided. Are there any variables whose sizes might change across platforms (e.g., implicitly sizing an array using a standard `int` instead of `size_t` or `int32_t`)?"*
  * *"Identify any circular dependencies or risky tight coupling between these `.c` files that could lead to undefined behavior."*

### 2. Automated Static Analysis Tools

**⚠️ CRITICAL: These tools catch bugs that runtime sanitizers CANNOT detect (e.g., uninitialized variables).**

* **`cppcheck`:** A tool designed specifically for C/C++ to catch out-of-bounds errors and uninitialized variables across multiple files without compiling.
  ```bash
  # Run cppcheck on the entire C codebase
  cppcheck --enable=all --inconclusive --force --suppress=missingIncludeSystem \
    -I/opt/homebrew/Cellar/netcdf/4.9.3_2/include \
    C/projects/ 2>&1 | tee /tmp/cppcheck_report.txt
  ```

* **`clang-tidy`:** A linter that helps catch modern C antipatterns.
  ```bash
  # Run clang-tidy on specific files
  clang-tidy C/projects/CARDAMOM_MODELS/DALEC/DALEC_OBSERVATION_OPERATORS/*.c \
    -checks='clang-analyzer-*,bugprone-*' \
    -- -I/opt/homebrew/Cellar/netcdf/4.9.3_2/include
  ```

* **Clang Static Analyzer:** Deep semantic analysis
  ```bash
  # Run clang static analyzer
  clang --analyze -Xanalyzer -analyzer-output=text \
    C/projects/CARDAMOM_MODELS/DALEC/DALEC_OBSERVATION_OPERATORS/DALEC_OBSERVATION_OPERATORS.c \
    -I/opt/homebrew/Cellar/netcdf/4.9.3_2/include 2>&1 | tee /tmp/clang_analyzer.txt
  ```

## Tier 2: Compilation Diagnostics & Sanitizers

*The goal here is to leverage modern compilers to catch dangerous behavior during the build process and immediately upon running.*

### 1. Strict Compiler Flags via Existing Build Script

We will update the project's compilation script, `BASH/CARDAMOM_COMPILE.sh`, to include aggressive warning flags. This forces the compiler to complain about anything remotely ambiguous.

* **Flags to add to `BASH/CARDAMOM_COMPILE.sh`:**
  * `-Wall -Wextra`: Turns on all standard warnings.
  * `-pedantic`: Warns if the code violates the strict ISO C standard (crucial for cross-platform compatibility).
  * `-Wconversion`: Warns about implicit type conversions that might lose data.

### 2. Implementation of Compilers Sanitizers

We will compile a special "Diagnostic Build" of CARDAMOM by further modifying `BASH/CARDAMOM_COMPILE.sh` to include LLVM/GCC sanitizers:

* **AddressSanitizer (ASan) & LeakSanitizer (LSan):** Add `-fsanitize=address`. If the code accesses memory out of bounds or leaks memory, the program will instantly crash and print the exact file and line number.
* **UndefinedBehaviorSanitizer (UBSan):** Add `-fsanitize=undefined`. This catches floating-point division by zero, integer overflows, and unaligned pointers.

## Tier 3: Dynamic Runtime Auditing & Performance Profiling

*The goal here is to run CARDAMOM with dedicated, representative scientific datasets to uncover deep-seated runtime bugs and performance bottlenecks that only appear under heavy load.*

### 1. Automated Stress Testing

Utilize the project's built-in testing resources to push the model to its limits across different environments:

* **Execution:** Use the python script `PYTHON/check_fun/CARDAMOM_CBF_NC_FILE_STRESS_TEST.py`.
* **Input Data:** Configure the stress test script to run using `CARDAMOM_DEMO_INPUT_FILE.cbf.nc` as the default driver file. This ensures the audits are performed on a representative, standardized scientific dataset.

### 2. Memory Profiling (Valgrind)

Run the stress test suite (compiled *without* sanitizers) through Valgrind for deep memory auditing: `valgrind --leak-check=full ./cardamom [arguments]` to confirm absolutely zero memory leaks over a long-running science simulation.

### 3. Performance Profiling

Since it's scientific code, execution speed matters. Ensure that updates to fix bugs don't accidentally slow down the math.

* **`gprof` or `perf`:** Compile `BASH/CARDAMOM_COMPILE.sh` with the `-pg` flag to generate a call graph. This will show exactly which functions within the stress test run are taking up the most CPU time.
* **Floating-Point Auditing:** Track "NaN" (Not a Number) or "Inf" (Infinity) cascades during the `CARDAMOM_DEMO_INPUT_FILE.cbf.nc` stress test, which often happen in complex models across different OS architectures.

---

## Appendix A: Lessons Learned from October 2026 Audit

### Case Study: Branch Comparison (Eren_HF_DF_28Jun23_STAGEMERGE vs main)

**Audit Goal:** Compare memory safety between stage branch and main branch.

**What Was Done:**
1. ✅ AddressSanitizer (ASan) + UndefinedBehaviorSanitizer (UBSan) testing
2. ✅ Stress test execution (CARDAMOM_CBF_NC_FILE_STRESS_TEST.py)
3. ✅ Manual code inspection after user hint about M_FIR
4. ✅ Git diff analysis between branches

**What Was Missed Initially:**
1. ❌ Tier 1 static analysis (cppcheck, clang-tidy)
2. ❌ AI-assisted whole codebase review
3. ❌ Systematic uninitialized variable checking

**Critical Bug Found (Only After User Hint):**
```c
// Location: C/projects/CARDAMOM_MODELS/DALEC/DALEC_OBSERVATION_OPERATORS/DALEC_OBSERVATION_OPERATORS.c:331
int n;              // Declared
D->M_FIR[n]=0;      // ❌ BUG: n is uninitialized! Random array index write
for (n=0;n<N;n++){  // n initialized here
    D->M_FIR[n]=D->M_FLUXES[D->nofluxes*n+O->FIR_flux];
}
```

**Why Runtime Sanitizers Missed It:**
- AddressSanitizer: Detects out-of-bounds access, BUT only if the random value of `n` happens to be out of bounds. If `n` randomly contains a valid index (e.g., 0-5), no error is triggered even though the code is wrong.
- UndefinedBehaviorSanitizer: Does NOT detect uninitialized reads (would need MemorySanitizer)
- LeakSanitizer: Not relevant for this bug type

**How Static Analysis Would Have Caught It:**
- `cppcheck --enable=all` flags: `uninitvar` (uninitialized variable)
- `clang-tidy -checks=clang-analyzer-core.uninitialized.Assign` 
- `clang --analyze` with uninitialized variable checker

**Key Lesson:** 
> **Runtime sanitizers are NOT sufficient.** They only catch issues that manifest during execution. Uninitialized variables may "work" 99% of the time if they randomly contain valid values, then cause catastrophic failure in production.

> **ALWAYS run Tier 1 static analysis FIRST.**

---

## Appendix B: Branch Comparison Workflow

**For comparing two git branches for memory safety:**

### Phase 1: Pre-Execution Static Analysis (REQUIRED)
```bash
# 1. Check out both branches and run static analyzers
for branch in main feature_branch; do
    git checkout $branch
    
    # Run cppcheck
    cppcheck --enable=all --inconclusive C/projects/ 2>&1 | \
        tee /tmp/cppcheck_${branch}.txt
    
    # Count issues
    grep -E "(error|warning|style)" /tmp/cppcheck_${branch}.txt | \
        wc -l > /tmp/cppcheck_${branch}_count.txt
done

# 2. Compare static analysis results
diff /tmp/cppcheck_main.txt /tmp/cppcheck_feature_branch.txt
```

### Phase 2: Compilation with Sanitizers
```bash
# Compile with all sanitizers
gcc -g -O1 -fsanitize=address,undefined \
    -fstack-protector-all -D_FORTIFY_SOURCE=2 \
    -Wall -Wextra -Wuninitialized -Wconversion \
    C/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.c \
    -o CARDAMOM_RUN_MODEL_ASAN.exe \
    -lm -lnetcdf -I/path/to/netcdf/include
```

### Phase 3: Runtime Testing
```bash
# Set aggressive ASAN options
export ASAN_OPTIONS="detect_leaks=1:detect_stack_use_after_return=1:check_initialization_order=1"

# Run stress test
python3 PYTHON/check_fun/CARDAMOM_CBF_NC_FILE_STRESS_TEST.py \
    CARDAMOM_DEMO_INPUT_FILE.cbf.nc 2>&1 | tee /tmp/stress_test.log
```

### Phase 4: Comparison Report
```bash
# Compare sanitizer outputs
echo "=== MAIN BRANCH ISSUES ==="
grep -E "(ERROR:|warning:|leak)" /tmp/asan_main.log | sort -u

echo "=== FEATURE BRANCH ISSUES ==="  
grep -E "(ERROR:|warning:|leak)" /tmp/asan_feature.log | sort -u

# Diff the results
diff <(grep "ERROR:" /tmp/asan_main.log | sort) \
     <(grep "ERROR:" /tmp/asan_feature.log | sort)
```

---

## Appendix C: Tool Availability by Platform

| Tool | macOS ARM64 | macOS x86 | Linux |
|------|-------------|-----------|-------|
| AddressSanitizer | ✅ Full | ✅ Full | ✅ Full |
| LeakSanitizer | ⚠️ Limited | ✅ Full | ✅ Full |
| UndefinedBehaviorSanitizer | ✅ Full | ✅ Full | ✅ Full |
| MemorySanitizer | ❌ No | ❌ No | ✅ Yes |
| Valgrind | ❌ No | ⚠️ Partial | ✅ Full |
| cppcheck | ✅ Yes | ✅ Yes | ✅ Yes |
| clang-tidy | ✅ Yes | ✅ Yes | ✅ Yes |

**Recommendation for macOS users:** Run Tier 1 static analysis locally, then use a Linux VM or CI for MemorySanitizer and Valgrind.

---

## Appendix D: AI Model Recommendations

Based on October 2026 audit experience:

**For initial audit response (user request):**
- **Sonnet 4.5 / 5**: Fast, good for standard sanitizer runs and git diffs
- **BUT**: May skip Tier 1 if not explicitly prompted

**For deep code review (after running this strategy):**
- **Opus 5**: Better at systematic code review
- Provide C/LLM_DEPENDENCIES.txt upfront
- Ask it to review headers before implementation files
- Prompt: "Review this C code for uninitialized variables, memory leaks, and undefined behavior. Follow the three-tier audit strategy in doc/cardamom_c_code_audit_strategy.md"

**For finding subtle bugs:**
- Static analyzers > AI models
- AI is good for: interpreting tool output, suggesting fixes, understanding intent
- AI is NOT a substitute for: cppcheck, clang-tidy, valgrind

**Recommended Prompt for Future Audits:**
```
I need to audit C code changes between two git branches for memory safety. 
Please follow the strategy in doc/cardamom_c_code_audit_strategy.md.

IMPORTANT: Start with Tier 1 static analysis using cppcheck and clang-tidy 
BEFORE running any runtime sanitizers. This catches uninitialized variables 
that AddressSanitizer cannot detect.

After static analysis, proceed to Tier 2 (sanitizers) and Tier 3 (stress testing).
```
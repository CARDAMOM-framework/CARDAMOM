"""
Multi-file test summary reporting for CARDAMOM CBF files.

This module provides utilities for summarizing test results across multiple
CBF files and tracking warnings during test execution.
"""

import sys
from contextlib import contextmanager
from io import StringIO
from typing import Dict, Any
from functools import wraps


def print_multi_file_summary(file_results: Dict[str, Dict[str, bool]]) -> None:
    """
    Print a summary of test results for multiple CBF files.

    Parameters
    ----------
    file_results : dict
        Dictionary mapping filename to result dict with keys:
        - 'passed': bool - whether all tests passed
        - 'has_warnings': bool - whether warnings were present

    Example
    -------
    >>> results = {
    ...     'file1.cbf.nc': {'passed': True, 'has_warnings': False},
    ...     'file2.cbf.nc': {'passed': False, 'has_warnings': True}
    ... }
    >>> print_multi_file_summary(results)
    """
    if not file_results:
        return

    # Print header
    print("\n" + "=" * 70)
    print("MULTI-FILE SUMMARY")
    print("=" * 70)

    # Track if any file has warnings
    any_warnings = False

    # Print result for each file
    for filename, result in file_results.items():
        passed = result.get('passed', False)
        has_warnings = result.get('has_warnings', False)

        if has_warnings:
            any_warnings = True

        if passed:
            print(f"✅ {filename} passed all tests")
        else:
            print(f"❌ {filename} did not pass all tests (see error messages above)")

    # Print warning note once at the end if any file has warnings
    if any_warnings:
        print(f"\n(⚠️  check individual file reports above for warning messages)")

    print("=" * 70 + "\n")


@contextmanager
def track_warnings_in_output():
    """
    Context manager that tracks whether warning or info messages were printed.

    Yields
    ------
    dict
        Dictionary with 'has_warnings' key (bool) indicating if warnings detected.

    Example
    -------
    >>> with track_warnings_in_output() as tracker:
    ...     print("Some output")
    ...     print("ℹ️ Info message")
    ...     print("More output")
    >>> print(tracker['has_warnings'])  # True

    >>> with track_warnings_in_output() as tracker:
    ...     print("Just normal output")
    >>> print(tracker['has_warnings'])  # False
    """
    # Create a tracker object to hold the warning state
    tracker = {'has_warnings': False}

    # Capture stdout
    original_stdout = sys.stdout
    captured_output = StringIO()

    class WarningTrackingStream:
        """Stream wrapper that tracks warning/info symbols."""

        def __init__(self, stream, tracker):
            self.stream = stream
            self.tracker = tracker

        def write(self, text):
            # Check for warning or info symbols
            if 'ℹ️' in text or '⚠️' in text:
                self.tracker['has_warnings'] = True
            # Write to both original stdout and capture buffer
            self.stream.write(text)
            captured_output.write(text)
            return len(text)

        def flush(self):
            self.stream.flush()
            captured_output.flush()

        def __getattr__(self, name):
            return getattr(self.stream, name)

    # Replace stdout with tracking stream
    sys.stdout = WarningTrackingStream(original_stdout, tracker)

    try:
        yield tracker
    finally:
        # Restore original stdout
        sys.stdout = original_stdout


def track_warnings_decorator(func):
    """
    Decorator that tracks warnings in function output and adds has_warnings to return value.

    The decorated function should return a dict. The decorator adds 'has_warnings' key.

    Parameters
    ----------
    func : callable
        Function to decorate (should return a dict)

    Returns
    -------
    callable
        Decorated function

    Example
    -------
    >>> @track_warnings_decorator
    ... def run_test():
    ...     print("ℹ️ Warning message")
    ...     return {'passed': True}
    >>> result = run_test()
    >>> print(result['has_warnings'])  # True
    """
    @wraps(func)
    def wrapper(*args, **kwargs):
        with track_warnings_in_output() as tracker:
            result = func(*args, **kwargs)

        # If function returns a dict, add has_warnings
        if isinstance(result, dict):
            result['has_warnings'] = tracker['has_warnings']

        return result

    return wrapper


if __name__ == "__main__":
    # Example usage
    print("Example 1: Summary with all tests passing, no warnings")
    results1 = {
        'file1.cbf.nc': {'passed': True, 'has_warnings': False},
        'file2.cbf.nc': {'passed': True, 'has_warnings': False},
    }
    print_multi_file_summary(results1)

    print("\nExample 2: Summary with mixed results and warnings")
    results2 = {
        'file1.cbf.nc': {'passed': True, 'has_warnings': False},
        'file2.cbf.nc': {'passed': False, 'has_warnings': True},
        'file3.cbf.nc': {'passed': True, 'has_warnings': True},
    }
    print_multi_file_summary(results2)

    print("\nExample 3: Using track_warnings_in_output context manager")
    with track_warnings_in_output() as tracker:
        print("Normal output")
        print("ℹ️ This is an info message")
        print("More normal output")
    print(f"Warnings detected: {tracker['has_warnings']}")

    print("\nExample 4: Using track_warnings_decorator")
    @track_warnings_decorator
    def sample_test():
        print("Running test...")
        print("⚠️ Warning found!")
        return {'passed': True, 'test_name': 'sample'}

    result = sample_test()
    print(f"Test result: {result}")

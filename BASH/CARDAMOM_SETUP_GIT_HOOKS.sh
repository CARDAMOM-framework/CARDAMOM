#!/bin/bash
# CARDAMOM_SETUP_GIT_HOOKS.sh - Initial setup script for CARDAMOM repository Git hooks

echo "🔧 Configuring CARDAMOM repository Git hooks..."
echo ""

# Make sure we're in the repository root
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
REPO_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"
cd "$REPO_ROOT"

# Make hooks executable
echo "Setting executable permissions on hooks..."
chmod +x .githooks/post-merge
chmod +x .githooks/post-checkout

# Configure Git hooks
echo "Configuring Git to use .githooks directory..."
git config core.hooksPath .githooks
echo "✅ Git hooks configured successfully!"
echo ""

echo "🎉 Setup complete!"
echo ""
echo "The following hooks are now active:"
echo "  • post-merge: Regenerates cbf.nc file after git pull on CARDAMOM branch"
echo "  • post-checkout: Regenerates cbf.nc file when switching to CARDAMOM branch"
echo ""
echo "Note: Other developers will need to run this script once after cloning:"
echo "  bash BASH/CARDAMOM_SETUP_GIT_HOOKS.sh"

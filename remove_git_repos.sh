#!/bin/bash

echo "Searching for git repositories in subdirectories..."

# Find all .git directories
git_dirs=$(find . -name ".git" -type d 2>/dev/null)

if [ -z "$git_dirs" ]; then
    echo "No git repositories found in subdirectories."
    exit 0
fi

echo "Found the following git repositories:"
echo "$git_dirs"
echo ""

# Ask for confirmation
read -p "Do you want to remove these git repositories? (y/N): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Removing git repositories..."
    for git_dir in $git_dirs; do
        echo "Removing: $git_dir"
        rm -rf "$git_dir"
    done
    echo "All git repositories have been removed."
else
    echo "Operation cancelled."
fi

#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Script to fix encoding issues in source files.
This script identifies files with encoding issues and re-encodes them as UTF-8.
It handles common encoding problems with French characters.
"""

import os
import re
import sys
from pathlib import Path
import chardet
import codecs

# Map of common incorrect encodings of French accented characters to their correct UTF-8 form
CHAR_CORRECTIONS = {
    b'\xe9': 'é',  # é
    b'\xe8': 'è',  # è
    b'\xea': 'ê',  # ê
    b'\xe0': 'à',  # à
    b'\xe7': 'ç',  # ç
    b'\xf9': 'ù',  # ù
    b'\xef': 'ï',  # ï
    b'\xee': 'î',  # î
    b'\xc9': 'É',  # É
    b'\xc8': 'È',  # È
    b'\xca': 'Ê',  # Ê
    b'\xc0': 'À',  # À
    b'\xc7': 'Ç',  # Ç
    b'\xd9': 'Ù',  # Ù
    b'\xcf': 'Ï',  # Ï
    b'\xce': 'Î',  # Î
    # Add more mappings as needed
}

# Extensions to process
EXTENSIONS = ['.cpp', '.h', '.hpp', '.c', '.cc']

# Directories to skip
SKIP_DIRS = ['build', '.git', 'doxygen']

def detect_encoding(file_path):
    """Detect the encoding of a file."""
    with open(file_path, 'rb') as f:
        data = f.read()
        result = chardet.detect(data)
        return result['encoding']

def fix_file_encoding(file_path):
    """Fix encoding issues in a file."""
    try:
        # Try to auto-detect encoding
        detected_encoding = detect_encoding(file_path)
        
        if detected_encoding is None:
            print(f"Unable to detect encoding for {file_path}")
            return False
        
        # Read the file with detected encoding
        with open(file_path, 'rb') as f:
            content = f.read()
        
        # Look for common encoding problems
        has_issues = False
        for bad_bytes, good_char in CHAR_CORRECTIONS.items():
            if bad_bytes in content:
                has_issues = True
                break
                
        if not has_issues:
            # Try to decode with detected encoding to check for errors
            try:
                content.decode(detected_encoding)
            except UnicodeDecodeError:
                has_issues = True

        if has_issues:
            # Try different encodings to properly decode the file
            decoded_content = None
            for encoding in ['latin-1', 'iso-8859-1', 'windows-1252', 'cp1252']:
                try:
                    decoded_content = content.decode(encoding)
                    break
                except UnicodeDecodeError:
                    continue
            
            if decoded_content is None:
                print(f"Error: Could not decode {file_path} with any known encoding")
                return False
            
            # Write the content back as UTF-8
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(decoded_content)
            
            print(f"Fixed encoding for {file_path}")
            return True
        else:
            print(f"No encoding issues found in {file_path}")
            return False
            
    except Exception as e:
        print(f"Error processing {file_path}: {e}")
        return False

def process_directory(dir_path):
    """Process all files in a directory recursively."""
    fixed_count = 0
    file_count = 0
    
    for root, dirs, files in os.walk(dir_path):
        # Skip directories we want to ignore
        dirs[:] = [d for d in dirs if d not in SKIP_DIRS]
        
        for file in files:
            file_path = os.path.join(root, file)
            file_ext = os.path.splitext(file)[1].lower()
            
            if file_ext in EXTENSIONS:
                file_count += 1
                if fix_file_encoding(file_path):
                    fixed_count += 1
    
    return file_count, fixed_count

if __name__ == "__main__":
    if len(sys.argv) > 1:
        root_dir = sys.argv[1]
    else:
        root_dir = "src"  # Default directory to process
    
    if not os.path.isdir(root_dir):
        print(f"Error: {root_dir} is not a valid directory")
        sys.exit(1)
    
    print(f"Fixing encoding issues in {root_dir}...")
    total_files, fixed_files = process_directory(root_dir)
    
    print(f"\nSummary:")
    print(f"Processed {total_files} files")
    print(f"Fixed encoding in {fixed_files} files")
    
    if fixed_files > 0:
        print("\nEncoding issues have been fixed. Now you should:")
        print("1. Clean your doxygen directory: rm -rf doxygen/*")
        print("2. Regenerate documentation: doxygen")
        print("3. Build your mkdocs site: mkdocs build")
    else:
        print("\nNo encoding issues found that needed fixing.") 
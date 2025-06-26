#!/usr/bin/env python3

import os
import sys
from pathlib import Path

try:
    import md4c
    use_md4c = True
except ImportError:
    import markdown
    use_md4c = False

INPUT_DIR = Path("./docs")
OUTPUT_DIR = Path("../build/public/content")
OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

def convert_md_to_html(md_text: str) -> str:
    if use_md4c:
        return md4c.md2html(md_text)
    else:
        return markdown.markdown(md_text)

def main():
    if not INPUT_DIR.exists():
        print(f"[INFO] No '{INPUT_DIR}' directory found. Nothing to convert.")
        sys.exit(0)

    md_files = list(INPUT_DIR.glob("*.md"))
    if not md_files:
        print(f"[INFO] No markdown files found in '{INPUT_DIR}'.")
        return

    print(f"[INFO] Rendering {len(md_files)} markdown files to HTML...")

    for md_file in md_files:
        try:
            html_text = convert_md_to_html(md_file.read_text(encoding="utf-8"))
            out_file = OUTPUT_DIR / (md_file.stem + ".html")
            out_file.write_text(html_text, encoding="utf-8")
            print(f"[OK] Converted: {md_file.name} -> {out_file.name}")
        except Exception as e:
            print(f"[ERROR] Failed to convert {md_file}: {e}")

if __name__ == "__main__":
    main()

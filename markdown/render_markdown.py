#!/usr/bin/env python3

import os
import sys
from pathlib import Path

try:
    import markdown
except ImportError:
    print("[ERROR] markdown module not found. Please install it with 'pip install markdown'")
    sys.exit(1)

PROJECT_ROOT = Path(__file__).resolve().parents[1]
INPUT_DIR = PROJECT_ROOT / "markdown/docs"
OUTPUT_DIR = PROJECT_ROOT / "build/public/content"
OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

BASE_STYLESHEET_LINK = '<link rel="stylesheet" href="/public/styles/baseStyle.css">'

def convert_md_to_html(md_text: str) -> str:
    body = markdown.markdown(md_text)
    html = f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>Document</title>
{BASE_STYLESHEET_LINK}
</head>
<body>
{body}
</body>
</html>"""
    return html

def main():
    if not INPUT_DIR.exists():
        print(f"[INFO] No '{INPUT_DIR}' directory found. Nothing to convert.")
        sys.exit(0)

    md_files = list(INPUT_DIR.glob("*.md"))
    html_files = set(f.stem + ".html" for f in md_files)

    # Convert markdown to HTML
    print(f"[INFO] Rendering {len(md_files)} markdown files to HTML...")
    for md_file in md_files:
        try:
            html_text = convert_md_to_html(md_file.read_text(encoding="utf-8"))
            out_file = OUTPUT_DIR / (md_file.stem + ".html")
            out_file.write_text(html_text, encoding="utf-8")
            print(f"[OK] Converted: {md_file.name} -> {out_file.name}")
        except Exception as e:
            print(f"[ERROR] Failed to convert {md_file}: {e}")

    # Delete orphaned HTML files
    existing_html_files = set(f.name for f in OUTPUT_DIR.glob("*.html"))
    orphaned_files = existing_html_files - html_files
    for orphan in orphaned_files:
        try:
            (OUTPUT_DIR / orphan).unlink()
            print(f"[CLEANUP] Removed orphaned HTML: {orphan}")
        except Exception as e:
            print(f"[ERROR] Failed to delete orphaned file {orphan}: {e}")

if __name__ == "__main__":
    main()

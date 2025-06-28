#!/usr/bin/env python3

import os
import sys
from pathlib import Path
from bs4 import BeautifulSoup
import markdown

PROJECT_ROOT = Path(__file__).resolve().parents[1]
INPUT_DIR = PROJECT_ROOT / "markdown/docs"
OUTPUT_DIR = PROJECT_ROOT / "build/public/content"
OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

def convert_md_to_html(md_text: str) -> str:
    raw_html = markdown.markdown(md_text, extensions=["extra"])
    soup = BeautifulSoup(raw_html, "html.parser")

    # Find the first heading for the <title>
    title_tag = soup.find(['h1', 'h2'])
    title = title_tag.get_text() if title_tag else "Document"

    # Group <h2> sections with following <p> tags into <section>
    output_body = BeautifulSoup("<body></body>", "html.parser")
    body_tag = output_body.body

    current_section = None
    for element in soup.contents:
        if element.name == "h2":
            current_section = output_body.new_tag("section")
            current_section.append(element)
            body_tag.append(current_section)
        elif current_section is not None:
            current_section.append(element)
        else:
            body_tag.append(element)

    full_html = f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>{title}</title>
<a href="/portfolio">My Portfolio</a>
<link rel="stylesheet" href="/public/styles/baseStyle.css">
<link rel="stylesheet" href="/public/styles/markdownStyle.css">
</head>
{str(body_tag)}
</html>"""

    return full_html



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

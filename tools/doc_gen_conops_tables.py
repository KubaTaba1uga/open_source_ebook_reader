
#!/usr/bin/env python3

import argparse
import re
from pathlib import Path

OBJECTIVE_PATTERN = re.compile(
    r'^\s*-\s*<a id="(OBJ-[\w-]+)"></a>\s*'
    r'`(OBJ-[\w-]+)`:?\s*(.+)$',
    re.MULTILINE,
)

LINK_PATTERN = re.compile(
    r'\[(OBJ-[\w-]+)\]\(([^)]+)\)'
)

RELATED_PATTERN = re.compile(
    r'^\*\*Related objectives\*\*:\s*(.*)$',
    re.MULTILINE,
)


def load_objectives(path):
    text = path.read_text(encoding="utf-8")
    objectives = {}

    for match in OBJECTIVE_PATTERN.finditer(text):
        anchor, obj_id, description = match.groups()

        if anchor != obj_id:
            raise ValueError(f"Anchor mismatch: {anchor} != {obj_id}")

        if obj_id in objectives:
            raise ValueError(f"Duplicate objective: {obj_id}")

        objectives[obj_id] = description.strip()

    return objectives


def replace_related_objectives(match, objectives):
    links = LINK_PATTERN.findall(match.group(1))

    if not links:
        raise ValueError("Related objectives field contains no links")

    rows = [
        "| Objective | Description |",
        "|---|---|",
    ]

    seen = set()

    for obj_id, url in links:
        if obj_id in seen:
            continue
        seen.add(obj_id)

        if obj_id not in objectives:
            raise ValueError(f"Unknown objective: {obj_id}")

        description = objectives[obj_id].replace("|", r"\|")
        rows.append(f"| [{obj_id}]({url}) | {description} |")

    return "\n".join(rows)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("stakeholders", type=Path)
    parser.add_argument("conops", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()

    objectives = load_objectives(args.stakeholders)
    conops = args.conops.read_text(encoding="utf-8")

    result = RELATED_PATTERN.sub(
        lambda match: replace_related_objectives(match, objectives),
        conops,
    )

    args.output.parent.mkdir(parents=True, exist_ok=True)
    
    if not args.output.exists() or args.output.read_text(encoding="utf-8") != result:
        args.output.write_text(result, encoding="utf-8")
    
    print(f"Loaded {len(objectives)} objectives")
    print(f"Generated {args.output}")


if __name__ == "__main__":
    main()

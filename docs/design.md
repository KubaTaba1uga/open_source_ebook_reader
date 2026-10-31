Design
=========

If you are interested in the design of the ebook reader you are in the good place.

The design documentation is splitted into multiple files in `design` directory, each file begins from
two numbers which identify order of reading the documents, after the numbers we have underscore and 
real name of the file.

The issue rises while writing the design docs that we have to refer to the same concept across multiple 
files that's why we introduce markers in form of HTML links like `<a id="MARKER-Data">MARKER-Data</a>` or 
`## STAKE-ProjectOwner`. The idea is that we create the link in one place in single file, and then whetever
we want to mention this single concept we use links [MARKER-Data](#MARKER-Data). 
So when you see sth like `UPPERCASE` dash `Data` this shouldn't be so strange to you.

Design documents:
- [Stakeholders](/design/00_stakeholders.md)
- [Concept of operations](/design/01_concept_of_operations.md)
- [Success criteria](/design/02_success_criteria.md)




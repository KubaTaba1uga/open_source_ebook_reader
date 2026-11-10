SELECT 1 FROM my_table LIMIT 1;

SELECT 'Valid rows:';
WITH pattern AS (
  SELECT substr(replace(hex(zeroblob(6554)), '00', '1234567890'), 1, 327680) AS p
)
SELECT COUNT(*) AS bad_rows
FROM my_table, pattern
WHERE CAST(my_table.data AS TEXT) == p;

SELECT 'Corrupted rows:';
WITH pattern AS (
  SELECT substr(replace(hex(zeroblob(6554)), '00', '1234567890'), 1, 327680) AS p
)
SELECT COUNT(*) AS bad_rows
FROM my_table, pattern
WHERE CAST(my_table.data AS TEXT) != p;

PRAGMA integrity_check;

# Decode infomask extension

A set of functions to decode infomask bits. The aim of this set of functions is
to ease interpreting hintbits and permit to query easily as every significant
hintbit is accessible as a boolean value.



## Usage

For example, the following query detects row that have the `XMIN_COMMITTED`
bit set :
```
WITH list_blocks AS (
  SELECT (generate_series(1,
            (pg_relation_size('test') / current_setting('block_size')::int)
         ) - 1)::int AS blk
)
SELECT '(' || blk || ',' || lp || ')' AS ctid,
       t_infomask, t_xmin, t_xmax,
       xactinfo.xmin_status, xactinfo.xmax_status
  FROM list_blocks,
       LATERAL heap_page_items(get_raw_page('test', list_blocks.blk)),
       LATERAL pg_get_xact_infomask(t_infomask) AS xactinfo
 WHERE xactinfo.xmin_committed;
```


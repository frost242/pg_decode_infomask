# Decode infomask extension

A set of PL/pgSQL functions to decode infomask bits.



## Usage

For example, the following query detects tuple where xmin=xmax (that's possible), and
XMIN_COMMITTED *AND* XMAX_INVALID are both set (which is impossible in this case... but happened in real
life) :
```
WITH suspicious_tuples AS (
  SELECT regexp_replace(ctid::text, '\((\d+),\d+\)','\1')::int AS blk, ctid
    FROM ONLY test
   WHERE xmin=xmax
)
SELECT l.ctid, pi.t_xmin, pi.t_xmax, im.xmin_status, im.xmax_status
  FROM suspicious_tuples l,
  heap_page_items(get_raw_page('test', l.blk)) pi,
  pg_get_xact_infomask(pi.t_infomask) im
 WHERE l.ctid = t_ctid
 AND im.xmin_committed AND im.xmax_invalid;
```



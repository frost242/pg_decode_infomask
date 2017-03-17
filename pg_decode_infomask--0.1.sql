/*
 * infomask/infomask2 decoding functions
 * for PostgreSQL 9.6
 *
 * author: Thomas Reiss
 *
 * all defined hintbits are given in form of a boolean columns, to permit the use
 * of a WHERE clause, for example to detect inconsistencies.
 *
 */

CREATE OR REPLACE FUNCTION pg_get_xact_infomask_bits(infomask int,
    OUT xmin_committed boolean,
    OUT xmin_invalid boolean,
    OUT xmin_frozen boolean,
    OUT xmax_committed boolean,
    OUT xmax_invalid boolean,
    OUT xmax_is_multi boolean
)
RETURNS record
LANGUAGE c COST 10
AS '$libdir/pg_decode_infomask', 'pg_get_xact_infomask_details';

CREATE OR REPLACE FUNCTION pg_get_infomask2_bits(infomask2 int,
    OUT natts integer,
    OUT keys_updated boolean,
    OUT hot_updated boolean,
    OUT heap_only_tuple boolean
)
RETURNS record
LANGUAGE c COST 10
AS '$libdir/pg_decode_infomask', 'pg_get_infomask2_details';


CREATE OR REPLACE FUNCTION pg_get_xact_infomask(infomask int,
    OUT xmin_committed boolean,
    OUT xmin_invalid boolean,
    OUT xmin_frozen boolean,
    OUT xmax_committed boolean,
    OUT xmax_invalid boolean,
    OUT xmax_is_multi boolean,
    OUT xmin_status text[],
    OUT xmax_status text[]
)
AS $$
SELECT xmin_committed, xmin_invalid, xmin_frozen,
       xmax_committed, xmax_invalid, xmax_is_multi,
       xmin_status.xmin_status, xmax_status.xmax_status
  FROM pg_get_xact_infomask_bits($1) xact,
       LATERAL ( SELECT array_agg(xminstatus) AS xmin_status FROM (	
                     SELECT 'COMMITTED' AS xminstatus WHERE xmin_committed
                     UNION ALL
                     SELECT 'INVALID' WHERE xmin_invalid
                     UNION ALL
                     SELECT 'FROZEN' WHERE xmin_frozen
                     ) s
       ) xmin_status,
       LATERAL ( SELECT array_agg(xmaxstatus) AS xmax_status FROM (	
                     SELECT 'COMMITTED' AS xmaxstatus WHERE xmax_committed
                     UNION ALL
                     SELECT 'INVALID' WHERE xmax_invalid
                     UNION ALL
                     SELECT 'MULTIXACT' WHERE xmax_is_multi
                     ) s
       ) xmax_status
$$ LANGUAGE sql;


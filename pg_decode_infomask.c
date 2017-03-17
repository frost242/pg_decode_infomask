#include "postgres.h"

#include "catalog/pg_type.h"

#include "funcapi.h"
#include "miscadmin.h"
#include "access/htup_details.h"

#define PG_GET_XACT_INFOMASK_DETAILS_COLS	5
#define PG_GET_LOCK_INFOMASK_DETAILS_COLS	9
#define PG_GET_INFOMASK2_DETAILS_COLS	4


PG_MODULE_MAGIC;

Datum pg_get_xact_infomask_details(PG_FUNCTION_ARGS);
Datum pg_get_lock_infomask_details(PG_FUNCTION_ARGS);
Datum pg_get_infomask2_details(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(pg_get_xact_infomask_details);
PG_FUNCTION_INFO_V1(pg_get_lock_infomask_details);
PG_FUNCTION_INFO_V1(pg_get_infomask2_details);

Datum
pg_get_xact_infomask_details(PG_FUNCTION_ARGS)
{
    TupleDesc    tupdesc;
    Datum        values[PG_GET_XACT_INFOMASK_DETAILS_COLS];
    bool         nulls[PG_GET_XACT_INFOMASK_DETAILS_COLS];
    int	         i = 0;
    int32        infomask = PG_GETARG_INT32(0);

    if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
        elog(ERROR, "return type must be a row type");

    tupdesc = CreateTemplateTupleDesc(PG_GET_XACT_INFOMASK_DETAILS_COLS, false);
    TupleDescInitEntry(tupdesc, ++i, "xmin_committed", BOOLOID, -1, 0);	    
    TupleDescInitEntry(tupdesc, ++i, "xmin_invalid", BOOLOID, -1, 0);	    
    TupleDescInitEntry(tupdesc, ++i, "xmin_frozen", BOOLOID, -1, 0);	    
    TupleDescInitEntry(tupdesc, ++i, "xmax_committed", BOOLOID, -1, 0);	    
    TupleDescInitEntry(tupdesc, ++i, "xmax_invalid", BOOLOID, -1, 0);	    
    BlessTupleDesc(tupdesc);

    MemSet(nulls, 0, sizeof(nulls));

    values[0] = BoolGetDatum((infomask & HEAP_XMIN_COMMITTED) != 0);
    values[1] = BoolGetDatum(((infomask & (HEAP_XMIN_INVALID | HEAP_XMIN_COMMITTED)) == HEAP_XMIN_INVALID));
    values[2] = BoolGetDatum((infomask & HEAP_XMIN_FROZEN) == HEAP_XMIN_FROZEN);
    values[3] = BoolGetDatum((infomask & HEAP_XMAX_COMMITTED) != 0);
    values[4] = BoolGetDatum((infomask & HEAP_XMAX_INVALID) != 0);

    /* Returns the record as Datum */
    PG_RETURN_DATUM(HeapTupleGetDatum(heap_form_tuple(tupdesc, values, nulls)));
}

Datum
pg_get_lock_infomask_details(PG_FUNCTION_ARGS)
{
    TupleDesc    tupdesc;
    Datum        values[PG_GET_LOCK_INFOMASK_DETAILS_COLS];
    bool         nulls[PG_GET_LOCK_INFOMASK_DETAILS_COLS];
    int	         i = 0;
    int32        infomask = PG_GETARG_INT32(0);

    if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
        elog(ERROR, "return type must be a row type");

    tupdesc = CreateTemplateTupleDesc(PG_GET_LOCK_INFOMASK_DETAILS_COLS, false);
    TupleDescInitEntry(tupdesc, ++i, "is_locked_only", BOOLOID, -1, 0);
    TupleDescInitEntry(tupdesc, ++i, "is_locked_upgraded", BOOLOID, -1, 0);
    TupleDescInitEntry(tupdesc, ++i, "is_shr_locked", BOOLOID, -1, 0);
    TupleDescInitEntry(tupdesc, ++i, "is_excl_locked", BOOLOID, -1, 0);
    TupleDescInitEntry(tupdesc, ++i, "is_keyshr_locked", BOOLOID, -1, 0);
    TupleDescInitEntry(tupdesc, ++i, "xmax_keyshr_lock", BOOLOID, -1, 0);
    TupleDescInitEntry(tupdesc, ++i, "xmax_excl_lock", BOOLOID, -1, 0);
    TupleDescInitEntry(tupdesc, ++i, "xmax_lock_only", BOOLOID, -1, 0);
    TupleDescInitEntry(tupdesc, ++i, "xmax_is_multi", BOOLOID, -1, 0);
    BlessTupleDesc(tupdesc);

    MemSet(nulls, 0, sizeof(nulls));

    i = 0;
    values[i++] = BoolGetDatum(HEAP_XMAX_IS_LOCKED_ONLY(infomask));
    values[i++] = BoolGetDatum(HEAP_LOCKED_UPGRADED(infomask));
    values[i++] = BoolGetDatum(HEAP_XMAX_IS_SHR_LOCKED(infomask));
    values[i++] = BoolGetDatum(HEAP_XMAX_IS_EXCL_LOCKED(infomask));
    values[i++] = BoolGetDatum(HEAP_XMAX_IS_KEYSHR_LOCKED(infomask));
    values[i++] = BoolGetDatum((infomask & HEAP_XMAX_KEYSHR_LOCK) != 0);
    values[i++] = BoolGetDatum((infomask & HEAP_XMAX_EXCL_LOCK) != 0);
    values[i++] = BoolGetDatum((infomask & HEAP_XMAX_LOCK_ONLY) != 0);
    values[i++] = BoolGetDatum((infomask & HEAP_XMAX_IS_MULTI) != 0);

    /* Returns the record as Datum */
    PG_RETURN_DATUM(HeapTupleGetDatum(heap_form_tuple(tupdesc, values, nulls)));
}

Datum
pg_get_infomask2_details(PG_FUNCTION_ARGS)
{
    TupleDesc    tupdesc;
    Datum        values[PG_GET_INFOMASK2_DETAILS_COLS];
    bool         nulls[PG_GET_INFOMASK2_DETAILS_COLS];
    int	         i = 0;
    int32        infomask2 = PG_GETARG_INT32(0);

    if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
        elog(ERROR, "return type must be a row type");

    tupdesc = CreateTemplateTupleDesc(PG_GET_INFOMASK2_DETAILS_COLS, false);
    TupleDescInitEntry(tupdesc, ++i, "natts", INT4OID, -1, 0);
    TupleDescInitEntry(tupdesc, ++i, "keys_updated", BOOLOID, -1, 0);
    TupleDescInitEntry(tupdesc, ++i, "hot_updated", BOOLOID, -1, 0);
    TupleDescInitEntry(tupdesc, ++i, "heap_only_tuple", BOOLOID, -1, 0);
    BlessTupleDesc(tupdesc);

    MemSet(nulls, 0, sizeof(nulls));

    i = 0;
    values[i++] = Int32GetDatum((infomask2 & HEAP_NATTS_MASK));
    values[i++] = BoolGetDatum((infomask2 & HEAP_KEYS_UPDATED) != 0);
    values[i++] = BoolGetDatum((infomask2 & HEAP_HOT_UPDATED) != 0);
    values[i++] = BoolGetDatum((infomask2 & HEAP_ONLY_TUPLE) != 0);

    /* Returns the record as Datum */
    PG_RETURN_DATUM(HeapTupleGetDatum(heap_form_tuple(tupdesc, values, nulls)));
}


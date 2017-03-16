#include "postgres.h"

#include "catalog/pg_type.h"

#include "funcapi.h"
#include "miscadmin.h"
#include "access/htup_details.h"

#define PG_GET_XACT_INFOMASK_DETAILS_COLS	6


PG_MODULE_MAGIC;

Datum
pg_get_xact_infomask_details(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(pg_get_xact_infomask_details);

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
    TupleDescInitEntry(tupdesc, ++i, "xmax_is_multi", BOOLOID, -1, 0);	    
    BlessTupleDesc(tupdesc);

    MemSet(nulls, 0, sizeof(nulls));

    values[0] = BoolGetDatum((infomask & HEAP_XMIN_COMMITTED) != 0);
    values[1] = BoolGetDatum(((infomask & (HEAP_XMIN_INVALID | HEAP_XMIN_COMMITTED)) == HEAP_XMIN_INVALID));
    values[2] = BoolGetDatum((infomask & HEAP_XMIN_FROZEN) == HEAP_XMIN_FROZEN);
    values[3] = BoolGetDatum((infomask & HEAP_XMAX_COMMITTED) != 0);
    values[4] = BoolGetDatum((infomask & HEAP_XMAX_INVALID) != 0);
    values[5] = BoolGetDatum((infomask & HEAP_XMAX_IS_MULTI) != 0);

    /* Returns the record as Datum */
    PG_RETURN_DATUM(HeapTupleGetDatum(heap_form_tuple(tupdesc, values, nulls)));
}



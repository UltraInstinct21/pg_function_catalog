/*
 * pg_function_catalog.c
 *
 * A PostgreSQL extension that provides function catalog information
 * to help prevent LLM hallucinations in SQL query generation.
 *
 * Author: Sarthi
 * Created: 16-02-2026
 */
/* Include PostgreSQL headers */
#include "postgres.h" /* Core PostgreSQL definitions */
#include "fmgr.h" /* Function manager interface */
#include "utils/builtins.h" /* Built-in type functions */
#include "executor/spi.h" /* Server Programming Interface */
#include "catalog/pg_type.h" /* Type OID constants */
#include "utils/snapmgr.h" /* Snapshot management */

/*
 * PG_MODULE_MAGIC is required in every extension
 * It's a version check that ensures the extension is compiled
 * for the same PostgreSQL version it's being loaded into
 */
PG_MODULE_MAGIC;
/*
 * PG_FUNCTION_INFO_V1 macro declares a version-1 C function
 * This creates metadata PostgreSQL needs to call the function
 */
PG_FUNCTION_INFO_V1(hello_catalog);
/*
 * hello_catalog - Simple test function
 *
 * Returns a text message to verify extension is working
 *
 * Arguments: none
 * Returns: text
 */
Datum
hello_catalog(PG_FUNCTION_ARGS)
{
 /*
 * cstring_to_text() converts C string to PostgreSQL text type
 * PG_RETURN_TEXT_P() returns a text value to PostgreSQL
 */
 char *message = "Hello from pg_function_catalog v2.0!";
 PG_RETURN_TEXT_P(cstring_to_text(message));
}


PG_FUNCTION_INFO_V1(get_function_info);
/*
 * get_function_info - Query pg_proc for function information
 *
 * Arguments:
 * function_name (text) - Name of function to look up
 *
 * Returns:
 * text - JSON-formatted function information (for now)
 *
 * Example:
 * SELECT get_function_info('substring');
 */

 Datum
get_function_info(PG_FUNCTION_ARGS)
{
 /* Step 1: Extract function argument */
 text *funcname_text;
 char *funcname_str;

 /* Check if argument is NULL (shouldn't happen with STRICT) */
 if (PG_ARGISNULL(0))
 PG_RETURN_NULL();

 /* Get the function name argument */
 funcname_text = PG_GETARG_TEXT_PP(0);

 /* Convert PostgreSQL text to C string */
 funcname_str = text_to_cstring(funcname_text);

 /* Step 2: Connect to SPI */
 int ret;
 ret = SPI_connect();
 if (ret != SPI_OK_CONNECT)
 {
 ereport(ERROR,
 (errcode(ERRCODE_CONNECTION_EXCEPTION),
 errmsg("SPI_connect failed: error code %d", ret)));
 }

 /* Step 3: Build SQL query */
 StringInfoData query;
 initStringInfo(&query);
 
 /*
 * Query explanation:
 * - pg_proc: main function catalog
 * - pg_namespace: schema names
 * - pg_description: function comments
 * - We use pg_get_function_* helper functions for formatting
 */
 appendStringInfo(&query,
 "SELECT "
 " p.proname AS func_name, " /* Function name */
 " n.nspname AS schema_name, " /* Schema name */
 " pg_get_function_result(p.oid) AS return_type, " /* Return type as text */
 " pg_get_function_arguments(p.oid) AS arguments, " /* Args as text */
 " COALESCE(d.description, 'No description') AS description, " /* Comment */
 " CASE p.prokind " /* Function kind */
 " WHEN 'f' THEN 'function' "
 " WHEN 'p' THEN 'procedure' "
 " WHEN 'a' THEN 'aggregate' "
 " WHEN 'w' THEN 'window' "
 " END AS func_type "
 "FROM pg_proc p "
 "JOIN pg_namespace n ON p.pronamespace = n.oid "
 "LEFT JOIN pg_description d ON p.oid = d.objoid AND d.objsubid = 0 "
 "WHERE p.proname = %s " /* Filter by name */
 " AND n.nspname = 'pg_catalog' " /* Only built-in functions */
 "LIMIT 1", /* Get first match */
 quote_literal_cstr(funcname_str)); /* SQL-safe quoting (prevents SQL injections) */

 /* Step 4: Execute query */
 ret = SPI_execute(query.data, true, 0); /* read-only, no row limit */

 if (ret != SPI_OK_SELECT)       /*if failed*/
 {
 SPI_finish();                  /*very important - use SPI_finish() before reporting error*/
 ereport(ERROR,
 (errcode(ERRCODE_INTERNAL_ERROR),
 errmsg("SPI_execute failed: error code %d", ret)));
 }

 /* Step 5: Process results */
 if (SPI_processed == 0)
 {
 /* No rows found */
 SPI_finish();

 /* Return a helpful message */
 StringInfoData result;
 initStringInfo(&result);
 appendStringInfo(&result,
 "Function '%s' not found in pg_catalog",
 funcname_str);

 PG_RETURN_TEXT_P(cstring_to_text(result.data));
 }

 /* Get the result tuple (row) */
 HeapTuple tuple = SPI_tuptable->vals[0];
 TupleDesc tupdesc = SPI_tuptable->tupdesc;

 /* Extract column values */
 bool isnull;

 char *func_name = SPI_getvalue(tuple, tupdesc, 1);
 char *schema_name = SPI_getvalue(tuple, tupdesc, 2);
 char *return_type = SPI_getvalue(tuple, tupdesc, 3);
 char *arguments = SPI_getvalue(tuple, tupdesc, 4);
 char *description = SPI_getvalue(tuple, tupdesc, 5);
 char *func_type = SPI_getvalue(tuple, tupdesc, 6);

 /* Step 6: Format result as JSON-like text */
 StringInfoData result;
 initStringInfo(&result);

 appendStringInfo(&result,
 "Function: %s\n"
 "Schema: %s\n"
 "Type: %s\n"
 "Returns: %s\n"
 "Arguments: %s\n"
 "Description: %s",
 func_name,
 schema_name,
 func_type,
 return_type ? return_type : "void",
 arguments ? arguments : "none",
 description);

 /* Step 7: Cleanup and return */
 SPI_finish();

 PG_RETURN_TEXT_P(cstring_to_text(result.data));
}



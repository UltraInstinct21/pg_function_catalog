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
 char *message = "Hello from pg_function_catalog!";
 PG_RETURN_TEXT_P(cstring_to_text(message));
}
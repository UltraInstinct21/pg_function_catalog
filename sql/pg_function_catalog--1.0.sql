-- sql/pg_function_catalog--1.0.sql  (v2.0)
-- Prevent script from being run directly in psql
\echo Use "CREATE EXTENSION pg_function_catalog" to load this file. \quit
-- Test function
CREATE FUNCTION hello_catalog()
RETURNS text
AS 'MODULE_PATHNAME', 'hello_catalog'
LANGUAGE C STRICT IMMUTABLE;
-- Main function to query function information
CREATE FUNCTION get_function_info(function_name text)
RETURNS text
AS 'MODULE_PATHNAME', 'get_function_info'
LANGUAGE C STRICT STABLE;
-- STABLE because it reads from system catalogs (not IMMUTABLE)
-- Add helpful comments
COMMENT ON FUNCTION get_function_info(text) IS
 'Returns detailed information about a PostgreSQL built-in function';
-- Example usage in comment
COMMENT ON FUNCTION get_function_info(text) IS
 'Example: SELECT get_function_info(''substring'')';


-- New table-returning function
CREATE FUNCTION pg_function_info_table(function_name text)
RETURNS TABLE (
 func_name text,
 schema_name text,
 return_type text,
 arguments text,
 description text,
 func_type text
)
AS 'MODULE_PATHNAME', 'pg_function_info_table'
LANGUAGE C STRICT STABLE;
COMMENT ON FUNCTION pg_function_info_table(text) IS
 'Returns function information as a table (can return multiple overloads)';
-- Convenience view for all built-in functions
CREATE VIEW pg_all_functions AS
SELECT DISTINCT
 p.proname AS function_name,
 n.nspname AS schema_name,
 CASE p.prokind
 WHEN 'f' THEN 'function'
 WHEN 'a' THEN 'aggregate'
 WHEN 'w' THEN 'window'
 WHEN 'p' THEN 'procedure'
 END AS function_type
FROM pg_proc p
JOIN pg_namespace n ON p.pronamespace = n.oid
WHERE n.nspname IN ('pg_catalog', 'information_schema')
ORDER BY p.proname;
COMMENT ON VIEW pg_all_functions IS
 'List of all PostgreSQL built-in functions';


 


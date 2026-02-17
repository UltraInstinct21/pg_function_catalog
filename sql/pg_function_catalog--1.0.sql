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
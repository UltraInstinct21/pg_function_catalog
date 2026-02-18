# pg_function_catalog

**Function catalog for LLM query generation**

`pg_function_catalog` is a PostgreSQL extension designed to provide a structured and detailed catalog of built-in functions. Its primary purpose is to assist Large Language Models (LLMs) in generating accurate SQL queries by exposing function signatures, return types, and descriptions in a machine-friendly format.

## Features

-   **Detailed Function Lookup**: Retrieve comprehensive information about any built-in PostgreSQL function.
-   **LLM-Friendly Output**: Returns function metadata in a formatted text block (similar to a JSON structure) or as a queryable table.
-   **Schema & Type Information**: Includes schema, argument types, return types, and function kinds (aggregate, window, etc.).
-   **Catalog View**: Provides a view listing all available functions.

## Installation

### Prerequisites

-   PostgreSQL (version 12+ recommended)
-   GCC (or compatible C compiler)
-   Make

### Build and Install

1.  Clone the repository:
    ```bash
    git clone https://github.com/your-username/pg_function_catalog.git
    cd pg_function_catalog
    ```

2.  Compile and install the extension:
    ```bash
    make
    sudo make install
    ```

3.  Enable the extension in your database:
    ```sql
    CREATE EXTENSION pg_function_catalog;
    ```

## Usage

### 1. Simple Check
Verify the installation with the hello world function:
```sql
SELECT hello_catalog();
-- Returns: Hello from pg_function_catalog v2.0!
```

### 2. Get Function Information (Text Format)
Retrieve a detailed text description of a function, useful for passing context to an LLM:

```sql
SELECT get_function_info('substring');
```

**Output:**
```text
Function: substring
Schema: pg_catalog
Type: function
Returns: text
Arguments: text, integer, integer
Description: extract substring
```

### 3. Get Function Information (Table Format)
Retrieve function details as a queryable table. This is useful if you need to join with other data or process specific fields.

```sql
SELECT * FROM pg_function_info_table('random');
```

| func\_name | schema\_name | return\_type | arguments | description | func\_type |
| :--- | :--- | :--- | :--- | :--- | :--- |
| random | pg\_catalog | double precision | | random number 0.0 <= x < 1.0 | function |

### 4. List All Functions
Query the `pg_all_functions` view to see all available built-in functions:

```sql
SELECT * FROM pg_all_functions 
WHERE function_name LIKE 'json%';
```

## Testing

To run the test suite, you can execute the `test/test_catalog.sql` file using `psql`:

```bash
psql -d postgres -f test/test_catalog.sql
```

## Internal Architecture

The extension is implemented in C using the PostgreSQL Server Programming Interface (SPI). It queries system catalogs like `pg_proc`, `pg_namespace`, and `pg_description` to aggregate function metadata.


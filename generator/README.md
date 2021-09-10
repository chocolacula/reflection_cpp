# rr_gen

Code generation tool for Easy Reflection based on Clang tools.

It combines parser info into one json entity and forward it to inja tempate engine.

format of an entity is

```
name, // full name including all namespaces
items[] // an array of fields/constants
  name,
  alias // is equal to <name> if RR_ALIAS attribute didn't parse
```
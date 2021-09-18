# rr_gen

Code generation tool for Easy Reflection based on Clang tools.

It combines parser info into one json entity and forward it to inja tempate engine.

format of an object entity is

```js
{
  name,  // full name including all namespaces
  template_parameters,
  bases [
    { access,
      name }  // full name of base class
  ],
  fields [
    { access,  // public, private or protected
      name,
      alias }  // is equal to <name> if ER_ALIAS attribute didn't parse
  ],
  static_fields [
    { access,
      name,
      alias }
  ]
}
```

format of an enum entity is

```js
{
  name, // including all namespaces
  constants [
    { name,
      alias }
  ]
}
```
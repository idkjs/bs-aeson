/** Provides functions for encoding a JSON data structure */;

/** The type of a encoder combinator */

type encoder('a) = 'a => Js.Json.t;

/** [null] is the singleton null JSON value */
[@bs.val]
external null: Js.Json.t = "null";

/** [string s] makes a JSON string of the [string] [s] */
external string: string => Js.Json.t = "%identity";

/** [float n] makes a JSON number of the [float] [n] */
external float: float => Js.Json.t = "%identity";

/** [int n] makes a JSON number of the [int] [n] */
external int: int => Js.Json.t = "%identity";

/** [int32 n] makes a JSON number of the [int32] [n] */
external int32: int32 => Js.Json.t = "%identity";

/** [int64 n] makes a JSON number of the [int64] [n] in the format of [high, low] where high is signed and low is unsigned */
external int64_to_array: int64 => Js.Json.t = "%identity";

/** [nativeint n] makes a JSON number of the [nativeint] [n] */
external nativeint: nativeint => Js.Json.t = "%identity";

/** [int64 n] makes a JSON number of the [int64] [n] in the format of a string */

let int64_string: Int64.t => Js.Json.t;

/** [bigint n] makes a JSON number of the [Bigint.t] [n] in the format of a string */

let bigint: Bigint.t => Js.Json.t;

/** [uint8 n] makes a JSON number of the [U.UInt8.t] [n] */

let uint8: U.UInt8.t => Js.Json.t;

/** [uint16 n] makes a JSON number of the [U.UInt8.t] [n] */

let uint16: U.UInt16.t => Js.Json.t;

/** [uint32 n] makes a JSON number of the [U.UInt32.t] [n] in the format of a string */

let uint32: U.UInt32.t => Js.Json.t;

/** [uint64 n] makes a JSON number of the [U.UInt64.t] [n] in the format of a string */

let uint64: U.UInt64.t => Js.Json.t;

/** [bool b] makes a JSON boolean of the [Js.bool] [b] */
external bool: bool => Js.Json.t = "%identity";

/** [nullable encoder option] returns either the encoded value or [Js.Json.null] */

let nullable: (encoder('a), option('a)) => Js.Json.t;

/** [withDefault default encoder option] returns the encoded value if present, oterwise [default] */

let withDefault: (Js.Json.t, encoder('a), option('a)) => Js.Json.t;

/** [dict d] makes a JSON objet of the [Js.Dict.t] [d] */
external dict: Js_dict.t(Js.Json.t) => Js.Json.t = "%identity";

/** [object_ props] makes a JSON objet of the [props] list of properties */

let object_: list((string, Js.Json.t)) => Js.Json.t;

/** [array a] makes a JSON array of the [Js.Json.t array] [a] */
external array: array(Js.Json.t) => Js.Json.t = "%identity";

/** [optional encoder a] returns the encoded value in Some, or null if Nothing */

let optional: (encoder('a), option('a)) => Js.Json.t;

/** [optionalField encoder fieldName a] returns the encoded value with the
    fieldName in a list, or an empty list if None */

let optionalField:
  (string, encoder('a), option('a)) => list((string, Js.Json.t));

/** [date d] makes a JSON string of the [date] [d] */

let date: encoder(Js_date.t);

/** [list encoder l] makes a JSON array of the [list] [l] using the given [encoder] */

let list: encoder('a) => encoder(list('a));

/** The functions below are specialized for specific array type which
    happened to be already JSON object in the BuckleScript runtime. Therefore
    they are more efficient (constant time rather than linear conversion). */;

let pair: (encoder('a), encoder('b), ('a, 'b)) => Js.Json.t;

let tuple2: (encoder('a), encoder('b), ('a, 'b)) => Js.Json.t;

let tuple3:
  (encoder('a), encoder('b), encoder('c), ('a, 'b, 'c)) => Js.Json.t;

let tuple4:
  (
    encoder('a),
    encoder('b),
    encoder('c),
    encoder('d),
    ('a, 'b, 'c, 'd)
  ) =>
  Js.Json.t;

let tuple5:
  (
    encoder('a),
    encoder('b),
    encoder('c),
    encoder('d),
    encoder('e),
    ('a, 'b, 'c, 'd, 'e)
  ) =>
  Js.Json.t;

let tuple6:
  (
    encoder('a),
    encoder('b),
    encoder('c),
    encoder('d),
    encoder('e),
    encoder('f),
    ('a, 'b, 'c, 'd, 'e, 'f)
  ) =>
  Js.Json.t;

let tuple7:
  (
    encoder('a),
    encoder('b),
    encoder('c),
    encoder('d),
    encoder('e),
    encoder('f),
    encoder('g),
    ('a, 'b, 'c, 'd, 'e, 'f, 'g)
  ) =>
  Js.Json.t;

let tuple8:
  (
    encoder('a),
    encoder('b),
    encoder('c),
    encoder('d),
    encoder('e),
    encoder('f),
    encoder('g),
    encoder('h),
    ('a, 'b, 'c, 'd, 'e, 'f, 'g, 'h)
  ) =>
  Js.Json.t;

let tuple9:
  (
    encoder('a),
    encoder('b),
    encoder('c),
    encoder('d),
    encoder('e),
    encoder('f),
    encoder('g),
    encoder('h),
    encoder('i),
    ('a, 'b, 'c, 'd, 'e, 'f, 'g, 'h, 'i)
  ) =>
  Js.Json.t;

let tuple10:
  (
    encoder('a),
    encoder('b),
    encoder('c),
    encoder('d),
    encoder('e),
    encoder('f),
    encoder('g),
    encoder('h),
    encoder('i),
    encoder('j),
    ('a, 'b, 'c, 'd, 'e, 'f, 'g, 'h, 'i, 'j)
  ) =>
  Js.Json.t;

let result: (encoder('a), encoder('b), Belt.Result.t('a, 'b)) => Js.Json.t;

let either:
  (encoder('l), encoder('r), Aeson_compatibility.Either.t('l, 'r)) =>
  Js.Json.t;

/** [singleEnumerator a] takes a value and returns an empty JSON array. Useful for encoding a single enumerator that matches Haskell aeson. */

let singleEnumerator: encoder('a);

/** [stringArray a] makes a JSON array of the [string array] [a] */
external stringArray: array(string) => Js.Json.t = "%identity";

/** [numberArray a] makes a JSON array of the [float array] [a] */
external numberArray: array(float) => Js.Json.t = "%identity";

/** [boolArray] makes a JSON array of the [bool array] [a] */
external boolArray: array(bool) => Js.Json.t = "%identity";

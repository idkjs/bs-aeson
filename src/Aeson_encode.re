type encoder('a) = 'a => Js.Json.t;

[@bs.val] external null: Js.Json.t = "null";
external string: string => Js.Json.t = "%identity";
external float: float => Js.Json.t = "%identity";
external int: int => Js.Json.t = "%identity";
external int32: int32 => Js.Json.t = "%identity";
external int64_to_array: int64 => Js.Json.t = "%identity";
external nativeint: nativeint => Js.Json.t = "%identity";
external bool: bool => Js.Json.t = "%identity";
external dict: Js_dict.t(Js.Json.t) => Js.Json.t = "%identity";

let int64_string = (x: Int64.t) => string(Int64.to_string(x));

let bigint = (x: Bigint.t) => string(Bigint.to_string(x));

let uint8 = (x: U.UInt8.t) => int(U.UInt8.toInt(x));

let uint16 = (x: U.UInt16.t) => int(U.UInt16.toInt(x));

/* underlying it is an int64 but is less than JS limit,
   haskell expects a numeric literal */
let uint32 = (x: U.UInt32.t) => int(U.UInt32.toInt(x));

let uint64 = (x: U.UInt64.t) => string(U.UInt64.toString(x));

let nullable = encode =>
  fun
  | None => null
  | Some(v) => encode(v);

let withDefault = (d, encode) =>
  fun
  | None => d
  | Some(v) => encode(v);

let optional = (encode, optionalValue) =>
  switch (optionalValue) {
  | Some(value) => encode(value)
  | None => null
  };

let optionalField = (fieldName, encode, optionalValue) =>
  switch (optionalValue) {
  | Some(value) => [(fieldName, encode(value))]
  | None => []
  };

/* Haskell aeson renders .000Z as Z */
let date = (d): Js.Json.t =>
  string(Js.String.replace(".000Z", "Z", Js_date.toISOString(d)));

let object_ = (props): Js.Json.t => props |> Js.Dict.fromList |> dict;

external array: array(Js.Json.t) => Js.Json.t = "%identity";

let list = (encode, l) => l |> List.map(encode) |> Array.of_list |> array;

let pair = (encodeT0, encodeT1, tuple) => {
  let (t0, t1) = tuple;
  array([|encodeT0(t0), encodeT1(t1)|]);
};

let tuple2 = pair;

let tuple3 = (encodeT0, encodeT1, encodeT2, tuple) => {
  let (t0, t1, t2) = tuple;
  array([|encodeT0(t0), encodeT1(t1), encodeT2(t2)|]);
};

let tuple4 = (encodeT0, encodeT1, encodeT2, encodeT3, tuple) => {
  let (t0, t1, t2, t3) = tuple;
  array([|encodeT0(t0), encodeT1(t1), encodeT2(t2), encodeT3(t3)|]);
};

let tuple5 = (encodeT0, encodeT1, encodeT2, encodeT3, encodeT4, tuple) => {
  let (t0, t1, t2, t3, t4) = tuple;
  array([|
    encodeT0(t0),
    encodeT1(t1),
    encodeT2(t2),
    encodeT3(t3),
    encodeT4(t4),
  |]);
};

let tuple6 =
    (encodeT0, encodeT1, encodeT2, encodeT3, encodeT4, encodeT5, tuple) => {
  let (t0, t1, t2, t3, t4, t5) = tuple;
  array([|
    encodeT0(t0),
    encodeT1(t1),
    encodeT2(t2),
    encodeT3(t3),
    encodeT4(t4),
    encodeT5(t5),
  |]);
};

let tuple7 =
    (
      encodeT0,
      encodeT1,
      encodeT2,
      encodeT3,
      encodeT4,
      encodeT5,
      encodeT6,
      tuple,
    ) => {
  let (t0, t1, t2, t3, t4, t5, t6) = tuple;
  array([|
    encodeT0(t0),
    encodeT1(t1),
    encodeT2(t2),
    encodeT3(t3),
    encodeT4(t4),
    encodeT5(t5),
    encodeT6(t6),
  |]);
};

let tuple8 =
    (
      encodeT0,
      encodeT1,
      encodeT2,
      encodeT3,
      encodeT4,
      encodeT5,
      encodeT6,
      encodeT7,
      tuple,
    ) => {
  let (t0, t1, t2, t3, t4, t5, t6, t7) = tuple;
  array([|
    encodeT0(t0),
    encodeT1(t1),
    encodeT2(t2),
    encodeT3(t3),
    encodeT4(t4),
    encodeT5(t5),
    encodeT6(t6),
    encodeT7(t7),
  |]);
};

let tuple9 =
    (
      encodeT0,
      encodeT1,
      encodeT2,
      encodeT3,
      encodeT4,
      encodeT5,
      encodeT6,
      encodeT7,
      encodeT8,
      tuple,
    ) => {
  let (t0, t1, t2, t3, t4, t5, t6, t7, t8) = tuple;
  array([|
    encodeT0(t0),
    encodeT1(t1),
    encodeT2(t2),
    encodeT3(t3),
    encodeT4(t4),
    encodeT5(t5),
    encodeT6(t6),
    encodeT7(t7),
    encodeT8(t8),
  |]);
};

let tuple10 =
    (
      encodeT0,
      encodeT1,
      encodeT2,
      encodeT3,
      encodeT4,
      encodeT5,
      encodeT6,
      encodeT7,
      encodeT8,
      encodeT9,
      tuple,
    ) => {
  let (t0, t1, t2, t3, t4, t5, t6, t7, t8, t9) = tuple;
  array([|
    encodeT0(t0),
    encodeT1(t1),
    encodeT2(t2),
    encodeT3(t3),
    encodeT4(t4),
    encodeT5(t5),
    encodeT6(t6),
    encodeT7(t7),
    encodeT8(t8),
    encodeT9(t9),
  |]);
};

let result = (encodeA, encodeB, e) =>
  switch (e) {
  | Belt.Result.Ok(a) => object_([("Ok", encodeA(a))])
  | Belt.Result.Error(b) => object_([("Error", encodeB(b))])
  };

let either = (encodeL, encodeR, e) =>
  switch (e) {
  | Aeson_compatibility.Either.Left(l) => object_([("Left", encodeL(l))])
  | Aeson_compatibility.Either.Right(r) => object_([("Right", encodeR(r))])
  };

let singleEnumerator = _x => array([||]);

external stringArray: array(string) => Js.Json.t = "%identity";
external numberArray: array(float) => Js.Json.t = "%identity";
external boolArray: array(bool) => Js.Json.t = "%identity";

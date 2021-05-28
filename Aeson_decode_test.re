open Jest;
open Expect;

module Test = {
  type default_case =
    | Float
    | Int
    | String
    | Null
    | Array
    | Object
    | Bool;

  type singleEnumerator =
    | SingleEnumerator;

  /* TODO: tests for this function */
  let test = (decoder, prefix) =>
    Aeson.(
      fun
      | Float =>
        test(prefix ++ "float", () =>
          expectFn(decoder, Encode.float(1.23)) |> toThrow
        )
      | Int =>
        test(prefix ++ "int", () =>
          expectFn(decoder, Encode.int(23)) |> toThrow
        )
      | String =>
        test(prefix ++ "string", () =>
          expectFn(decoder, Encode.string("test")) |> toThrow
        )
      | Null =>
        test(prefix ++ "null", () =>
          expectFn(decoder, Encode.null) |> toThrow
        )
      | Array =>
        test(prefix ++ "array", () =>
          expectFn(decoder, Encode.array([||])) |> toThrow
        )
      | Object =>
        test(prefix ++ "object", () =>
          expectFn(decoder, Encode.object_([])) |> toThrow
        )
      | Bool =>
        test(prefix ++ "bool", () =>
          expectFn(decoder, Encode.bool(true)) |> toThrow
        )
    );

  let rec throws = (~prefix="", decoder) =>
    fun
    | [] => ()
    | [first, ...rest] => {
        test(decoder, prefix, first);
        throws(decoder, ~prefix, rest);
      };
};

let () = {
  describe("bool", () =>{
    open Aeson;
    open Decode;

    test("bool", () =>
      expect @@ bool(Encode.bool(true)) |> toEqual(true)
    );

    test("bool - false", () =>
      expect @@ bool(Encode.bool(false)) |> toEqual(false)
    );
    Test.throws(bool, [Float, Int, String, Null, Array, Object]);
  });

  describe("float", () =>{
    open Aeson;
    open! Decode;

    test("float", () =>
      expect @@ float(Encode.float(1.23)) |> toEqual(1.23)
    );
    test("int", () =>
      expect @@ float(Encode.int(23)) |> toEqual(23.)
    );

    Test.throws(float, [Bool, String, Null, Array, Object]);
  });

  describe("int", () =>{
    open Aeson;
    open! Decode;

    test("int", () =>
      expect @@ int(Encode.int(23)) |> toEqual(23)
    );

    test("int > 32-bit", () =>{
      /* Use %raw since integer literals > Int32.max_int overflow without warning */
      let big_int = [%raw "2147483648"];
      expect @@ int(Encode.int(big_int)) |> toEqual(big_int);
    });

    Test.throws(int, [Bool, Float, String, Null, Array, Object]);
  });

  describe("int32", () =>{
    open Aeson;
    open! Decode;

    test("int32", () =>
      expect @@
      int32(Encode.int32(Int32.of_int(23)))
      |> toEqual(Int32.of_int(23))
    );

    test("int32", () =>
      expect @@
      int32(Encode.int32(Int32.of_int(-23223)))
      |> toEqual(Int32.of_int(-23223))
    );
  });

  describe("int64", () =>{
    open Aeson;
    open! Decode;

    test("int64", () =>
      expect @@
      int64(Aeson.Encode.string("23"))
      |> toEqual(Int64.of_int(23))
    );

    test("int64", () =>
      expect @@ int64(Aeson.Encode.int(23)) |> toEqual(Int64.of_int(23))
    );

    test("int64", () =>
      expect @@
      int64(Aeson.Encode.int(23))
      |> toEqual @@
      Obj.magic([|0, 23|])
    );
  });

  describe("int64_of_array", () =>{
    open Aeson;
    open! Decode;

    test("int64_of_array", () =>
      expect @@
      int64_of_array(Encode.int64_to_array(Int64.of_int(23)))
      |> toEqual(Int64.of_int(23))
    );

    test("int64_of_array", () =>
      expect @@
      int64_of_array(
        Encode.int64_to_array(Int64.of_string("9223372036854775807")),
      )
      |> toEqual(Int64.of_string("9223372036854775807"))
    );

    test("int64_of_array", () =>
      expect @@
      int64_of_array(
        Encode.int64_to_array(Int64.of_string("-9223372036854775807")),
      )
      |> toEqual(Int64.of_string("-9223372036854775807"))
    );
  });

  describe("uint8", () =>{
    open Aeson;
    open! Decode;

    test("uint8", () =>
      expect @@
      uint8(Encode.uint8(U.UInt8.ofInt(23)))
      |> toEqual(U.UInt8.ofInt(23))
    );

    test("uint8", () =>
      expect @@
      uint8(Encode.uint8(U.UInt8.ofInt(255)))
      |> toEqual(U.UInt8.ofInt(255))
    );
  });

  describe("uint16", () =>{
    open Aeson;
    open! Decode;

    test("uint16", () =>
      expect @@
      uint16(Encode.uint16(U.UInt16.ofInt(23)))
      |> toEqual(U.UInt16.ofInt(23))
    );

    test("uint16", () =>
      expect @@
      uint16(Encode.uint16(U.UInt16.ofInt(1233)))
      |> toEqual(U.UInt16.ofInt(1233))
    );
  });

  describe("uint32", () =>{
    open Aeson;
    open! Decode;

    test("uint32", () =>
      expect @@
      uint32(Encode.uint32(U.UInt32.ofInt(23)))
      |> toEqual(U.UInt32.ofInt(23))
    );

    test("uint32", () =>
      expect @@
      uint32(Encode.uint32(U.UInt32.ofInt(23223)))
      |> toEqual(U.UInt32.ofInt(23223))
    );
  });

  describe("uint64", () =>{
    open Aeson;
    open! Decode;

    test("uint64", () =>
      expect @@
      uint64(Encode.uint64(U.UInt64.ofInt(23)))
      |> toEqual(U.UInt64.ofInt(23))
    );

    test("uint64", () =>
      expect @@
      uint64(Encode.uint64(U.UInt64.ofInt(26423)))
      |> toEqual(U.UInt64.ofInt(26423))
    );
  });

  describe("bigint", () =>
    open Aeson;
    open! Decode;

    test("bigint", () =>
      expect @@
      bigint(Encode.bigint(Bigint.of_int(23)))
      |> toEqual(Bigint.of_int(23))
    );

    test("bigint", () =>
      expect @@
      bigint(Encode.bigint(Bigint.of_int(26423)))
      |> toEqual(Bigint.of_int(26423))
    );
  );

  describe("string", () =>
    open Aeson;
    open! Decode;

    test("string", () =>
      expect @@ string(Encode.string("test")) |> toEqual("test")
    );

    Test.throws(string, [Bool, Float, Int, Null, Array, Object]);
  );

  describe("date", () =>
    open Aeson;
    open! Decode;
    let nowString = "2017-12-08T06:03:22Z";
    let now = Js_date.fromString(nowString);

    test("date", () =>
      expect @@ date(Encode.date(now)) |> toEqual(now)
    );
  );

  describe("nullable", () =>
    open Aeson;
    open! Decode;

    test("int -> int", () =>
      expect @@
      (nullable(int))(Encode.int(23))
      |> toEqual(Js.Null.return(23))
    );
    test("null -> int", () =>
      expect @@ (nullable(int))(Encode.null) |> toEqual(Js.null)
    );

    test("bool -> bool", () =>
      expect @@
      nullable(bool, Encode.bool(true))
      |> toEqual(Js.Null.return(true))
    );
    test("float -> float", () =>
      expect @@
      nullable(float, Encode.float(1.23))
      |> toEqual(Js.Null.return(1.23))
    );
    test("string -> string", () =>
      expect @@
      nullable(string, Encode.string("test"))
      |> toEqual(Js.Null.return("test"))
    );
    test("null -> null", () =>
      expect @@ nullable(nullAs(Js.null), Encode.null) |> toEqual(Js.null)
    );

    Test.throws(nullable(int), [Bool, Float, String, Array, Object]);
    Test.throws(nullable(bool), [Int]);
  );

  describe("nullAs", () =>
    open Aeson;
    open Decode;

    test("as 0 - null", () =>
      expect @@ (nullAs(0))(Encode.null) |> toEqual(0)
    );

    test("as Js.null", () =>
      expect(nullAs(Js.null, Encode.null)) |> toEqual(Js.null)
    );
    test("as None", () =>
      expect(nullAs(None, Encode.null)) |> toEqual(None)
    );
    test("as Some _", () =>
      expect(nullAs(Some("foo"), Encode.null)) |> toEqual(Some("foo"))
    );

    Test.throws(nullAs(0), [Bool, Float, Int, String, Array, Object]);
  );

  describe("array", () =>
    open Aeson;
    open! Decode;

    test("array", () =>
      expect @@ (array(int))(Encode.array([||])) |> toEqual([||])
    );

    test("array bool", () =>
      expect @@
      array(bool, Js.Json.parseExn({| [true, false, true] |}))
      |> toEqual([|true, false, true|])
    );
    test("array float", () =>
      expect @@
      array(float, Js.Json.parseExn({| [1, 2, 3] |}))
      |> toEqual([|1., 2., 3.|])
    );
    test("array int", () =>
      expect @@
      array(int, Js.Json.parseExn({| [1, 2, 3] |}))
      |> toEqual([|1, 2, 3|])
    );
    test("array string", () =>
      expect @@
      array(string, Js.Json.parseExn({| ["a", "b", "c"] |}))
      |> toEqual([|"a", "b", "c"|])
    );
    test("array nullAs", () =>
      expect @@
      array(nullAs(Js.null), Js.Json.parseExn({| [null, null, null] |}))
      |> toEqual([|Js.null, Js.null, Js.null|])
    );
    test("array int -> array bool", () =>
      expectFn(array(bool), Js.Json.parseExn({| [1, 2, 3] |})) |> toThrow
    );

    Test.throws(array(int), [Bool, Float, Int, String, Null, Object]);
  );

  describe("list", () =>
    open Aeson;
    open! Decode;

    test("array", () =>
      expect @@ (list(int))(Encode.array([||])) |> toEqual([])
    );

    test("list bool", () =>
      expect @@
      list(bool, Js.Json.parseExn({| [true, false, true] |}))
      |> toEqual([true, false, true])
    );
    test("list float", () =>
      expect @@
      list(float, Js.Json.parseExn({| [1, 2, 3] |}))
      |> toEqual([1., 2., 3.])
    );
    test("list int", () =>
      expect @@
      list(int, Js.Json.parseExn({| [1, 2, 3] |}))
      |> toEqual([1, 2, 3])
    );
    test("list string", () =>
      expect @@
      list(string, Js.Json.parseExn({| ["a", "b", "c"] |}))
      |> toEqual(["a", "b", "c"])
    );
    test("list nullAs", () =>
      expect @@
      list(nullAs(Js.null), Js.Json.parseExn({| [null, null, null] |}))
      |> toEqual([Js.null, Js.null, Js.null])
    );
    test("array int -> list bool", () =>
      expectFn(list(bool), Js.Json.parseExn({| [1, 2, 3] |})) |> toThrow
    );

    Test.throws(list(int), [Bool, Float, Int, String, Null, Object]);
  );

  describe("pair", () =>
    open Aeson;
    open! Decode;

    test("pair string int", () =>
      expect @@
      pair(string, int, Js.Json.parseExn({| ["a", 3] |}))
      |> toEqual(("a", 3))
    );
    test("pair int int", () =>
      expect @@
      pair(int, int, Js.Json.parseExn({| [4, 3] |}))
      |> toEqual((4, 3))
    );
    test("pair missing", () =>
      expectFn(pair(int, int), Js.Json.parseExn({| [4] |})) |> toThrow
    );
    test("pair too large", () =>
      expectFn(pair(int, int), Js.Json.parseExn({| [3, 4, 5] |})) |> toThrow
    );
    test("pair bad left type", () =>
      expectFn(pair(int, int), Js.Json.parseExn({| ["3", 4] |})) |> toThrow
    );
    test("pair bad right type", () =>
      expectFn(pair(string, string), Js.Json.parseExn({| ["3", 4] |}))
      |> toThrow
    );
  );

  describe("tuple3", () =>
    open Aeson;
    open! Decode;

    test("tuple3 string int string", () =>
      expect @@
      tuple3(string, int, string, Js.Json.parseExn({| ["a", 3, "b"] |}))
      |> toEqual(("a", 3, "b"))
    );
  );

  describe("tuple4", () =>
    open Aeson;
    open! Decode;

    test("tuple4 string int string bool", () =>
      expect @@
      tuple4(
        string,
        int,
        string,
        bool,
        Js.Json.parseExn({| ["a", 3, "b", true] |}),
      )
      |> toEqual(("a", 3, "b", true))
    );
  );

  describe("tuple5", () =>
    open Aeson;
    open! Decode;

    test("tuple5 string int string bool int", () =>
      expect @@
      tuple5(
        string,
        int,
        string,
        bool,
        int,
        Js.Json.parseExn({| ["a", 3, "b", true, 98] |}),
      )
      |> toEqual(("a", 3, "b", true, 98))
    );
  );

  describe("tuple6", () =>
    open Aeson;
    open! Decode;

    test("tuple6 string int string bool int string", () =>
      expect @@
      tuple6(
        string,
        int,
        string,
        bool,
        int,
        string,
        Js.Json.parseExn({| ["a", 3, "b", true, 98, "sleepy"] |}),
      )
      |> toEqual(("a", 3, "b", true, 98, "sleepy"))
    );
  );

  describe("tuple7", () =>
    open Aeson;
    open! Decode;

    test("tuple7 string int string bool int string int", () =>
      expect @@
      tuple7(
        string,
        int,
        string,
        bool,
        int,
        string,
        int,
        Js.Json.parseExn({| ["a", 3, "b", true, 98, "sleepy", 100] |}),
      )
      |> toEqual(("a", 3, "b", true, 98, "sleepy", 100))
    );
  );

  describe("tuple8", () =>
    open Aeson;
    open! Decode;

    test("tuple8 string int string bool int string int string", () =>
      expect @@
      tuple8(
        string,
        int,
        string,
        bool,
        int,
        string,
        int,
        string,
        Js.Json.parseExn(
          {| ["a", 3, "b", true, 98, "sleepy", 100, "bedtime"] |},
        ),
      )
      |> toEqual(("a", 3, "b", true, 98, "sleepy", 100, "bedtime"))
    );
  );

  describe("tuple9", () =>
    open Aeson;
    open! Decode;

    test("tuple9 string int string bool int string int string bool", () =>
      expect @@
      tuple9(
        string,
        int,
        string,
        bool,
        int,
        string,
        int,
        string,
        bool,
        Js.Json.parseExn(
          {| ["a", 3, "b", true, 98, "sleepy", 100, "bedtime", false] |},
        ),
      )
      |> toEqual(("a", 3, "b", true, 98, "sleepy", 100, "bedtime", false))
    );
  );

  describe("tuple10", () =>
    open Aeson;
    open! Decode;

    test("tuple10 string int string bool int string int string bool int", () =>
      expect @@
      tuple10(
        string,
        int,
        string,
        bool,
        int,
        string,
        int,
        string,
        bool,
        int,
        Js.Json.parseExn(
          {| ["a", 3, "b", true, 98, "sleepy", 100, "bedtime", false, 22] |},
        ),
      )
      |> toEqual(("a", 3, "b", true, 98, "sleepy", 100, "bedtime", false, 22))
    );
  );

  describe("singleEnumerator", () =>
    open Aeson;
    open! Decode;

    test("singleEnumerator", () =>
      expect @@
      singleEnumerator(Test.SingleEnumerator, Encode.array([||]))
      |> toEqual(Test.SingleEnumerator)
    );
  );

  describe("dict", () =>
    open Aeson;
    open! Decode;

    test("object", () =>
      expect @@ dict(int, Encode.object_([])) |> toEqual(Js.Dict.empty())
    );

    test("dict bool", () =>
      expect @@
      dict(bool, Js.Json.parseExn({| { "a": true, "b": false } |}))
      |> toEqual(Obj.magic([%obj {a: true, b: false}]))
    );
    test("dict float", () =>
      expect @@
      dict(float, Js.Json.parseExn({| { "a": 1.2, "b": 2.3 } |}))
      |> toEqual(Obj.magic([%obj {a: 1.2, b: 2.3}]))
    );
    test("dict int", () =>
      expect @@
      dict(int, Js.Json.parseExn({| { "a": 1, "b": 2 } |}))
      |> toEqual(Obj.magic([%obj {a: 1, b: 2}]))
    );
    test("dict string", () =>
      expect @@
      dict(string, Js.Json.parseExn({| { "a": "x", "b": "y" } |}))
      |> toEqual(Obj.magic([%obj {a: "x", b: "y"}]))
    );
    test("dict nullAs", () =>
      expect @@
      dict(
        nullAs(Js.null),
        Js.Json.parseExn({| { "a": null, "b": null } |}),
      )
      |> toEqual(Obj.magic([%obj {a: Js.null, b: Js.null}]))
    );
    test("dict null -> dict string", () =>
      expectFn(
        dict(string),
        Js.Json.parseExn({| { "a": null, "b": null } |}),
      )
      |> toThrow
    );

    Test.throws(dict(int), [Bool, Float, Int, String, Null, Array]);
  );

  describe("field", () =>
    open Aeson;
    open! Decode;

    test("field bool", () =>
      expect @@
      field("b", bool, Js.Json.parseExn({| { "a": true, "b": false } |}))
      |> toEqual(false)
    );
    test("field float", () =>
      expect @@
      field("b", float, Js.Json.parseExn({| { "a": 1.2, "b": 2.3 } |}))
      |> toEqual(2.3)
    );
    test("field int", () =>
      expect @@
      field("b", int, Js.Json.parseExn({| { "a": 1, "b": 2 } |}))
      |> toEqual(2)
    );
    test("field string", () =>
      expect @@
      field("b", string, Js.Json.parseExn({| { "a": "x", "b": "y" } |}))
      |> toEqual("y")
    );
    test("field nullAs", () =>
      expect @@
      field(
        "b",
        nullAs(Js.null),
        Js.Json.parseExn({| { "a": null, "b": null } |}),
      )
      |> toEqual(Js.null)
    );
    test("field null -> field string", () =>
      expectFn(
        field("b", string),
        Js.Json.parseExn({| { "a": null, "b": null } |}),
      )
      |> toThrow
    );

    Test.throws(
      field("foo", int),
      [Bool, Float, Int, String, Null, Array, Object],
    );
  );

  describe("at", () =>
    open Aeson;
    open! Decode;

    test("at bool", () =>
      expect @@
      at(
        ["a", "x", "y"],
        bool,
        Js.Json.parseExn(
          {| {
        "a": { "x" : { "y" : false } },
        "b": false
      } |},
        ),
      )
      |> toEqual(false)
    );
    test("field nullAs", () =>
      expect @@
      at(
        ["a", "x"],
        nullAs(Js.null),
        Js.Json.parseExn(
          {| {
        "a": { "x" : null },
        "b": null
      } |},
        ),
      )
      |> toEqual(Js.null)
    );

    Test.throws(
      at(["foo", "bar"], int),
      [Bool, Float, Int, String, Null, Array, Object],
    );
  );

  describe("optional", () =>
    open Aeson;
    open! Decode;

    test("bool -> int", () =>
      expect @@ (optional(int))(Encode.bool(true)) |> toEqual(None)
    );
    test("float -> int", () =>
      expect @@ (optional(int))(Encode.float(1.23)) |> toEqual(None)
    );
    test("int -> int", () =>
      expect @@ (optional(int))(Encode.int(23)) |> toEqual(Some(23))
    );
    test("int32 -> int32", () =>
      expect @@
      (optional(int32))(Encode.int32(Int32.of_int(23)))
      |> toEqual(Some(Int32.of_int(23)))
    );
    test("int64 -> int64", () =>
      expect @@
      (optional(int64_of_array))(Encode.int64_to_array(Int64.of_int(64)))
      |> toEqual(Some(Int64.of_int(64)))
    );
    test("string -> int", () =>
      expect @@ (optional(int))(Encode.string("test")) |> toEqual(None)
    );
    test("null -> int", () =>
      expect @@ (optional(int))(Encode.null) |> toEqual(None)
    );
    test("array -> int", () =>
      expect @@ (optional(int))(Encode.array([||])) |> toEqual(None)
    );
    test("object -> int", () =>
      expect @@ (optional(int))(Encode.object_([])) |> toEqual(None)
    );

    test("bool -> bool ", () =>
      expect @@ optional(bool, Encode.bool(true)) |> toEqual(Some(true))
    );
    test("float -> float", () =>
      expect @@ optional(float, Encode.float(1.23)) |> toEqual(Some(1.23))
    );
    test("string -> string", () =>
      expect @@
      optional(string, Encode.string("test"))
      |> toEqual(Some("test"))
    );
    test("null -> null", () =>
      expect @@
      optional(nullAs(Js.null), Encode.null)
      |> toEqual(Some(Js.null))
    );
    test("int -> bool", () =>
      expect @@ (optional(bool))(Encode.int(1)) |> toEqual(None)
    );

    test("optional field", () =>
      expect @@
      optional(field("x", int), Js.Json.parseExn({| { "x": 2} |}))
      |> toEqual(Some(2))
    );
    test("optional field - incorrect type", () =>
      expect @@
      optional(field("x", int), Js.Json.parseExn({| { "x": 2.3} |}))
      |> toEqual(None)
    );
    test("optional field - no such field", () =>
      expect @@
      optional(field("y", int), Js.Json.parseExn({| { "x": 2} |}))
      |> toEqual(None)
    );
    test("field optional", () =>
      expect @@
      field("x", optional(int), Js.Json.parseExn({| { "x": 2} |}))
      |> toEqual(Some(2))
    );
    test("field optional - incorrect type", () =>
      expect @@
      field("x", optional(int), Js.Json.parseExn({| { "x": 2.3} |}))
      |> toEqual(None)
    );
    test("field optional - no such field", () =>
      expectFn(
        field("y", optional(int)),
        Js.Json.parseExn({| { "x": 2} |}),
      )
      |> toThrow
    );
  );

  describe("optionalField", () =>
    open Aeson;
    open! Decode;

    test("optionalField", () =>
      expect @@
      optionalField("x", int, Js.Json.parseExn({| { "x": 2} |}))
      |> toEqual(Some(2))
    );

    test("optionalField - null returns None", () =>
      expect @@
      optionalField("x", int, Js.Json.parseExn({| { "x": null} |}))
      |> toEqual(None)
    );

    test("optionalField - null returns None", () =>
      expect @@
      optionalField("x", string, Js.Json.parseExn({| { "x": null} |}))
      |> toEqual(None)
    );

    test("optionalField - field does not exist", () =>
      expect @@
      optionalField("y", int, Js.Json.parseExn({| { "x": 2} |}))
      |> toEqual(None)
    );

    test("field optional - no such field", () =>
      expectFn(
        optionalField("x", string),
        Js.Json.parseExn({| { "x": 2} |}),
      )
      |> toThrow
    );
  );

  describe("oneOf", () =>
    open Aeson;
    open! Decode;

    test("object with field", () =>
      expect @@
      (oneOf([int, field("x", int)]))(Js.Json.parseExn({| { "x": 2} |}))
      |> toEqual(2)
    );
    test("int", () =>
      expect @@
      (oneOf([int, field("x", int)]))(Encode.int(23))
      |> toEqual(23)
    );

    Test.throws(
      oneOf([int, field("x", int)]),
      [Bool, Float, String, Null, Array, Object],
    );
  );

  describe("result", () =>
    open Aeson;
    open! Decode;

    test("Ok", () =>
      expect @@
      (result(int, string))(Js.Json.parseExn({| {"Error": "hello"} |}))
      |> toEqual(Belt.Result.Error("hello"))
    );

    test("Error", () =>
      expect @@
      (result(int, string))(Js.Json.parseExn({| {"Ok": 2} |}))
      |> toEqual(Belt.Result.Ok(2))
    );
  );

  describe("either", () =>
    open Aeson;
    open! Decode;

    test("Right", () =>
      expect @@
      (either(int, string))(Js.Json.parseExn({| {"Right": "hello"} |}))
      |> toEqual(Compatibility.Either.Right("hello"))
    );

    test("Left", () =>
      expect @@
      (either(int, string))(Js.Json.parseExn({| {"Left": 2} |}))
      |> toEqual(Compatibility.Either.Left(2))
    );
  );

  describe("tryEither", () =>
    open Aeson;
    open! Decode;

    test("object with field", () =>
      expect @@
      (tryEither(int, field("x", int)))(Js.Json.parseExn({| { "x": 2} |}))
      |> toEqual(2)
    );
    test("int", () =>
      expect @@
      (tryEither(int, field("x", int)))(Encode.int(23))
      |> toEqual(23)
    );

    Test.throws(
      tryEither(int, field("x", int)),
      [Bool, Float, String, Null, Array, Object],
    );
  );

  describe("withDefault", () =>
    open Aeson;
    open! Decode;

    test("bool", () =>
      expect @@ (withDefault(0, int))(Encode.bool(true)) |> toEqual(0)
    );
    test("float", () =>
      expect @@ (withDefault(0, int))(Encode.float(1.23)) |> toEqual(0)
    );
    test("int", () =>
      expect @@ (withDefault(0, int))(Encode.int(23)) |> toEqual(23)
    );
    test("string", () =>
      expect @@ (withDefault(0, int))(Encode.string("test")) |> toEqual(0)
    );
    test("null", () =>
      expect @@ (withDefault(0, int))(Encode.null) |> toEqual(0)
    );
    test("array", () =>
      expect @@ (withDefault(0, int))(Encode.array([||])) |> toEqual(0)
    );
    test("object", () =>
      expect @@ (withDefault(0, int))(Encode.object_([])) |> toEqual(0)
    );
  );

  describe("map", () =>
    open Aeson;
    open! Decode;

    test("int", () =>
      expect @@ (int |> map((+)(2)))(Encode.int(23)) |> toEqual(25)
    );

    Test.throws(
      int |> map((+)(2)),
      [Bool, Float, String, Null, Array, Object],
    );
  );

  describe("andThen", () =>
    open Aeson;
    open! Decode;

    test("int -> int", () =>
      expect @@ (int |> andThen(_ => int))(Encode.int(23)) |> toEqual(23)
    );

    test("int -> int andThen float", () =>
      expect @@ (int |> andThen(_ => float))(Encode.int(23)) |> toEqual(23.)
    );
    test("int -> float andThen int", () =>
      expect @@ (float |> andThen(_ => int))(Encode.int(23)) |> toEqual(23)
    );

    Test.throws(
      ~prefix="int andThen int ",
      int |> andThen(_ => int),
      [Bool, Float, String, Null, Array, Object],
    );
    Test.throws(
      ~prefix="float andThen int ",
      float |> andThen(_ => int),
      [Float],
    );
    Test.throws(~prefix="int to ", int |> andThen(_ => float), [Float]);
  );

  describe("composite expressions", () =>
    open Aeson;
    open! Decode;

    test("dict array array int", () =>
      expect @@
      dict(
        array(array(int)),
        Js.Json.parseExn({| { "a": [[1, 2], [3]], "b": [[4], [5, 6]] } |}),
      )
      |> toEqual(
           Obj.magic(
             [%obj {a: [|[|1, 2|], [|3|]|], b: [|[|4|], [|5, 6|]|]}],
           ),
         )
    );
    test("dict array array int - heterogenous structure", () =>
      expectFn(
        dict(array(array(int))),
        Js.Json.parseExn({| { "a": [[1, 2], [true]], "b": [[4], [5, 6]] } |}),
      )
      |> toThrow
    );
    test("dict array array int - heterogenous structure 2", () =>
      expectFn(
        dict(array(array(int))),
        Js.Json.parseExn({| { "a": [[1, 2], "foo"], "b": [[4], [5, 6]] } |}),
      )
      |> toThrow
    );
    test("field", () =>
      let json = Js.Json.parseExn({| { "foo": [1, 2, 3], "bar": "baz" } |});
      expect @@
      (field("foo", array(int), json), field("bar", string, json))
      |> toEqual(([|1, 2, 3|], "baz"));
    );
  );
};
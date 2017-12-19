open Jest
open Expect
open! Aeson.Encode

module Test = struct
  type singleEnumerator =
    | SingleEnumerator
end
  
let _ =

test "null" (fun () ->
  expect null |> toEqual @@ Obj.magic Js.null);

test "string" (fun () ->
  expect @@ string "foo" |> toEqual @@ Obj.magic "foo");

test "date - non-float time" (fun () ->
  let nowString = "2017-12-08T06:03:22Z" in
  let now = Js_date.fromString nowString in
  expect @@ date now |> toEqual @@ Obj.magic nowString);

test "date - float time" (fun () ->
  let nowString = "2017-12-08T06:03:22.123Z" in
  let now = Js_date.fromString nowString in
  expect @@ date now |> toEqual @@ Obj.magic nowString);

test "float" (fun () ->
  expect @@ float 1.23 |> toEqual @@ Obj.magic 1.23);

test "int" (fun () ->
  expect @@ int 23 |> toEqual @@ Obj.magic 23);

test "boolean" (fun () ->
  expect @@ boolean Js.true_ |> toEqual @@ Obj.magic Js.true_);

test "dict - empty" (fun () ->
  expect @@ dict @@ Js.Dict.empty () |> toEqual @@ Obj.magic @@ Js.Dict.empty ());

test "dict - simple" (fun () ->
  let o = Js.Dict.empty () in
  Js.Dict.set o "x" (int 42);
  expect @@ dict o |> toEqual @@ Obj.magic o);

test "object_ - empty" (fun () ->
  expect @@ object_ @@ [] |> toEqual @@ Obj.magic @@ Js.Dict.empty ());

test "object_ - simple" (fun () ->
  expect @@ object_ [("x", int 42)] |> toEqual @@ Obj.magic (Js.Dict.fromList [("x", 42)]));

test "array int" (fun () ->
  expect @@ array ([|1;2;3|] |> Array.map int) |> toEqual @@ Obj.magic [|1;2;3|]);

test "list int" (fun () ->
  expect @@ list int [1;2;3] |> toEqual @@ Obj.magic [|1;2;3|]);

test "singleEnumerator typeParameterRef0" (fun () ->
  expect @@ singleEnumerator Test.SingleEnumerator |> toEqual @@ Obj.magic [||]);

test "stringArray" (fun () ->
  expect @@ stringArray [|"a";"b"|]  |> toEqual @@ Obj.magic [|"a";"b"|]);

test "nubmerArray" (fun () ->
  expect @@ numberArray [|0.;4.|] |> toEqual @@ Obj.magic [|0;4|]);

test "booleanArray" (fun () ->
  expect @@ booleanArray [|Js.true_;Js.false_|] |> toEqual @@ Obj.magic [|Js.true_;Js.false_|]);

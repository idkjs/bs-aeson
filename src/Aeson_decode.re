[@bs.new]
external _unsafeCreateUninitializedArray: int => array('a) = "Array";

let _isInteger = value =>
  Js.Float.isFinite(value) && Js.Math.floor_float(value) === value;

type decoder('a) = Js.Json.t => 'a;

exception DecodeError(string);

let bool = json =>
  if (Js.typeof(json) == "boolean") {
    (Obj.magic(json: Js.Json.t): bool);
  } else {
    raise @@ DecodeError("Expected boolean, got " ++ Js.Json.stringify(json));
  };

let float = json =>
  if (Js.typeof(json) == "number") {
    (Obj.magic(json: Js.Json.t): float);
  } else {
    raise @@ DecodeError("Expected number, got " ++ Js.Json.stringify(json));
  };

let int = json => {
  let f = float(json);
  if (_isInteger(f)) {
    (Obj.magic(f: float): int);
  } else {
    raise @@ DecodeError("Expected int, got " ++ Js.Json.stringify(json));
  };
};

let int32 = json => {
  let f = float(json);
  if (_isInteger(f)) {
    (Obj.magic(f: float): int32);
  } else {
    raise @@ DecodeError("Expected int32, got " ++ Js.Json.stringify(json));
  };
};

let nativeint = json => {
  let f = float(json);
  if (_isInteger(f)) {
    (Obj.magic(f: float): nativeint);
  } else {
    raise @@
    DecodeError("Expected nativeint, got " ++ Js.Json.stringify(json));
  };
};

let string = json =>
  if (Js.typeof(json) == "string") {
    (Obj.magic(json: Js.Json.t): string);
  } else {
    raise @@ DecodeError("Expected string, got " ++ Js.Json.stringify(json));
  };

let explode = s => List.init(String.length(s), String.get(s));

let isDigit = char => {
  let c = Char.code(char);
  c >= 48 && c <= 57;
};

let isStringOfDigits = s =>
  if (String.length(s) === 0) {
    false;
  } else {
    List.fold_right((c, x) => isDigit(c) && x, explode(s), true);
  };

let uint8 = json => {
  let f = float(json);
  if (_isInteger(f)) {
    U.UInt8.ofInt(Obj.magic(f: float): int);
  } else {
    raise @@ DecodeError("Expected int, got " ++ Js.Json.stringify(json));
  };
};

let uint16 = json => {
  let f = float(json);
  if (_isInteger(f)) {
    U.UInt16.ofInt(Obj.magic(f: float): int);
  } else {
    raise @@ DecodeError("Expected int, got " ++ Js.Json.stringify(json));
  };
};

let uint32 = json =>
  switch (int(json)) {
  | v => U.UInt32.ofInt(v)
  | exception (DecodeError(_)) =>
    raise @@
    DecodeError("Expected U.UInt32.t, got " ++ Js.Json.stringify(json))
  };

let uint64 = json =>
  if (Js.typeof(json) == "string") {
    let source: string = Obj.magic(json: Js.Json.t);
    switch (U.UInt64.ofString(source)) {
    | Some(s) => s
    | None => raise @@ DecodeError("Expected U.UInt64.t, got " ++ source)
    };
  } else {
    raise @@
    DecodeError("Expected U.UInt64.t, got " ++ Js.Json.stringify(json));
  };

let int64_of_string = json =>
  if (Js.typeof(json) == "string") {
    let source: string = Obj.magic(json: Js.Json.t);

    if (isStringOfDigits(source)) {
      Int64.of_string(source);
    } else {
      raise @@ DecodeError("Expected int64, got " ++ source);
    };
  } else {
    raise @@ DecodeError("Expected int64, got " ++ Js.Json.stringify(json));
  };

let bigint = json =>
  if (Js.typeof(json) == "string") {
    let source: string = Obj.magic(json: Js.Json.t);

    if (isStringOfDigits(source)) {
      Bigint.of_string(source);
    } else {
      raise @@ DecodeError("Expected Bigint.t, got " ++ source);
    };
  } else {
    raise @@
    DecodeError("Expected Bigint.t, got " ++ Js.Json.stringify(json));
  };

let date = json =>
  if (Js.typeof(json) == "string") {
    let source: string = Obj.magic(json: Js.Json.t);
    let encodedDate = Js_date.fromString(source);
    if (Js_float.isNaN(Js_date.getTime(encodedDate))) {
      raise @@ DecodeError("Expected date, got " ++ source);
    } else {
      encodedDate;
    };
  } else {
    raise @@ DecodeError("Expected date, got " ++ Js.Json.stringify(json));
  };

let nullable = (decode, json) =>
  if ((Obj.magic(json): Js.null('a)) === Js.null) {
    Js.null;
  } else {
    Js.Null.return(decode(json));
  };

/* TODO: remove this? */
let nullAs = (value, json) =>
  if ((Obj.magic(json): Js.null('a)) === Js.null) {
    value;
  } else {
    raise @@ DecodeError("Expected null, got " ++ Js.Json.stringify(json));
  };

let array = (decode, json) =>
  if (Js.Array.isArray(json)) {
    let source: array(Js.Json.t) = Obj.magic(json: Js.Json.t);
    let length = Js.Array.length(source);
    let target = _unsafeCreateUninitializedArray(length);
    for (i in 0 to length - 1) {
      let value = decode(Array.unsafe_get(source, i));
      Array.unsafe_set(target, i, value);
    };
    target;
  } else {
    raise @@ DecodeError("Expected array, got " ++ Js.Json.stringify(json));
  };

let list = (decode, json) => json |> array(decode) |> Array.to_list;

let pair = (left, right, json) =>
  if (Js.Array.isArray(json)) {
    let source: array(Js.Json.t) = Obj.magic(json: Js.Json.t);
    let length = Js.Array.length(source);
    if (length == 2) {
      (
        left(Array.unsafe_get(source, 0)),
        right(Array.unsafe_get(source, 1)),
      );
    } else {
      raise @@
      DecodeError(
        {j|Expected array of length 2, got array of length $length|j},
      );
    };
  } else {
    raise @@ DecodeError("Expected array, got " ++ Js.Json.stringify(json));
  };

let tuple2 = pair;

let tuple3 = (first, second, third, json) =>
  if (Js.Array.isArray(json)) {
    let source: array(Js.Json.t) = Obj.magic(json: Js.Json.t);
    let length = Js.Array.length(source);
    if (length == 3) {
      (
        first(Array.unsafe_get(source, 0)),
        second(Array.unsafe_get(source, 1)),
        third(Array.unsafe_get(source, 2)),
      );
    } else {
      raise @@
      DecodeError(
        {j|Expected array of length 3, got array of length $length|j},
      );
    };
  } else {
    raise @@ DecodeError("Expected array, got " ++ Js.Json.stringify(json));
  };

let tuple4 = (first, second, third, fourth, json) =>
  if (Js.Array.isArray(json)) {
    let source: array(Js.Json.t) = Obj.magic(json: Js.Json.t);
    let length = Js.Array.length(source);
    if (length == 4) {
      (
        first(Array.unsafe_get(source, 0)),
        second(Array.unsafe_get(source, 1)),
        third(Array.unsafe_get(source, 2)),
        fourth(Array.unsafe_get(source, 3)),
      );
    } else {
      raise @@
      DecodeError(
        {j|Expected array of length 4, got array of length $length|j},
      );
    };
  } else {
    raise @@ DecodeError("Expected array, got " ++ Js.Json.stringify(json));
  };

let tuple5 = (first, second, third, fourth, fifth, json) =>
  if (Js.Array.isArray(json)) {
    let source: array(Js.Json.t) = Obj.magic(json: Js.Json.t);
    let length = Js.Array.length(source);
    if (length == 5) {
      (
        first(Array.unsafe_get(source, 0)),
        second(Array.unsafe_get(source, 1)),
        third(Array.unsafe_get(source, 2)),
        fourth(Array.unsafe_get(source, 3)),
        fifth(Array.unsafe_get(source, 4)),
      );
    } else {
      raise @@
      DecodeError(
        {j|Expected array of length 5, got array of length $length|j},
      );
    };
  } else {
    raise @@ DecodeError("Expected array, got " ++ Js.Json.stringify(json));
  };

let tuple6 = (first, second, third, fourth, fifth, sixth, json) =>
  if (Js.Array.isArray(json)) {
    let source: array(Js.Json.t) = Obj.magic(json: Js.Json.t);
    let length = Js.Array.length(source);
    if (length == 6) {
      (
        first(Array.unsafe_get(source, 0)),
        second(Array.unsafe_get(source, 1)),
        third(Array.unsafe_get(source, 2)),
        fourth(Array.unsafe_get(source, 3)),
        fifth(Array.unsafe_get(source, 4)),
        sixth(Array.unsafe_get(source, 5)),
      );
    } else {
      raise @@
      DecodeError(
        {j|Expected array of length 6, got array of length $length|j},
      );
    };
  } else {
    raise @@ DecodeError("Expected array, got " ++ Js.Json.stringify(json));
  };

let tuple7 = (first, second, third, fourth, fifth, sixth, seventh, json) =>
  if (Js.Array.isArray(json)) {
    let source: array(Js.Json.t) = Obj.magic(json: Js.Json.t);
    let length = Js.Array.length(source);
    if (length == 7) {
      (
        first(Array.unsafe_get(source, 0)),
        second(Array.unsafe_get(source, 1)),
        third(Array.unsafe_get(source, 2)),
        fourth(Array.unsafe_get(source, 3)),
        fifth(Array.unsafe_get(source, 4)),
        sixth(Array.unsafe_get(source, 5)),
        seventh(Array.unsafe_get(source, 6)),
      );
    } else {
      raise @@
      DecodeError(
        {j|Expected array of length 7, got array of length $length|j},
      );
    };
  } else {
    raise @@ DecodeError("Expected array, got " ++ Js.Json.stringify(json));
  };

let tuple8 =
    (first, second, third, fourth, fifth, sixth, seventh, eighth, json) =>
  if (Js.Array.isArray(json)) {
    let source: array(Js.Json.t) = Obj.magic(json: Js.Json.t);
    let length = Js.Array.length(source);
    if (length == 8) {
      (
        first(Array.unsafe_get(source, 0)),
        second(Array.unsafe_get(source, 1)),
        third(Array.unsafe_get(source, 2)),
        fourth(Array.unsafe_get(source, 3)),
        fifth(Array.unsafe_get(source, 4)),
        sixth(Array.unsafe_get(source, 5)),
        seventh(Array.unsafe_get(source, 6)),
        eighth(Array.unsafe_get(source, 7)),
      );
    } else {
      raise @@
      DecodeError(
        {j|Expected array of length 8, got array of length $length|j},
      );
    };
  } else {
    raise @@ DecodeError("Expected array, got " ++ Js.Json.stringify(json));
  };

let tuple9 =
    (first, second, third, fourth, fifth, sixth, seventh, eighth, ninth, json) =>
  if (Js.Array.isArray(json)) {
    let source: array(Js.Json.t) = Obj.magic(json: Js.Json.t);
    let length = Js.Array.length(source);
    if (length == 9) {
      (
        first(Array.unsafe_get(source, 0)),
        second(Array.unsafe_get(source, 1)),
        third(Array.unsafe_get(source, 2)),
        fourth(Array.unsafe_get(source, 3)),
        fifth(Array.unsafe_get(source, 4)),
        sixth(Array.unsafe_get(source, 5)),
        seventh(Array.unsafe_get(source, 6)),
        eighth(Array.unsafe_get(source, 7)),
        ninth(Array.unsafe_get(source, 8)),
      );
    } else {
      raise @@
      DecodeError(
        {j|Expected array of length 9, got array of length $length|j},
      );
    };
  } else {
    raise @@ DecodeError("Expected array, got " ++ Js.Json.stringify(json));
  };

let tuple10 =
    (
      first,
      second,
      third,
      fourth,
      fifth,
      sixth,
      seventh,
      eighth,
      ninth,
      tenth,
      json,
    ) =>
  if (Js.Array.isArray(json)) {
    let source: array(Js.Json.t) = Obj.magic(json: Js.Json.t);
    let length = Js.Array.length(source);
    if (length == 10) {
      (
        first(Array.unsafe_get(source, 0)),
        second(Array.unsafe_get(source, 1)),
        third(Array.unsafe_get(source, 2)),
        fourth(Array.unsafe_get(source, 3)),
        fifth(Array.unsafe_get(source, 4)),
        sixth(Array.unsafe_get(source, 5)),
        seventh(Array.unsafe_get(source, 6)),
        eighth(Array.unsafe_get(source, 7)),
        ninth(Array.unsafe_get(source, 8)),
        tenth(Array.unsafe_get(source, 9)),
      );
    } else {
      raise @@
      DecodeError(
        {j|Expected array of length 10, got array of length $length|j},
      );
    };
  } else {
    raise @@ DecodeError("Expected array, got " ++ Js.Json.stringify(json));
  };

let singleEnumerator = (a, json) =>
  if (Js.Array.isArray(json)) {
    let source: array(Js.Json.t) = Obj.magic(json: Js.Json.t);
    let length = Js.Array.length(source);
    if (length == 0) {
      a;
    } else {
      raise @@
      DecodeError(
        {j|Expected array of length 0, got array of length $length|j},
      );
    };
  } else {
    raise @@ DecodeError("Expected array, got " ++ Js.Json.stringify(json));
  };

let dict = (decode, json) =>
  if (Js.typeof(json) == "object"
      && !Js.Array.isArray(json)
      && !((Obj.magic(json): Js.null('a)) === Js.null)) {
    let source: Js.Dict.t(Js.Json.t) = Obj.magic(json: Js.Json.t);
    let keys = Js.Dict.keys(source);
    let l = Js.Array.length(keys);
    let target = Js.Dict.empty();
    for (i in 0 to l - 1) {
      let key = Array.unsafe_get(keys, i);
      let value = decode(Js.Dict.unsafeGet(source, key));
      Js.Dict.set(target, key, value);
    };
    target;
  } else {
    raise @@ DecodeError("Expected object, got " ++ Js.Json.stringify(json));
  };

let field = (key, decode, json) =>
  if (Js.typeof(json) == "object"
      && !Js.Array.isArray(json)
      && !((Obj.magic(json): Js.null('a)) === Js.null)) {
    let dict: Js.Dict.t(Js.Json.t) = Obj.magic(json: Js.Json.t);
    switch (Js.Dict.get(dict, key)) {
    | Some(value) => decode(value)
    | None => raise @@ DecodeError({j|Expected field '$(key)'|j})
    };
  } else {
    raise @@ DecodeError("Expected object, got " ++ Js.Json.stringify(json));
  };

let optionalField = (key, decode, json) =>
  if (Js.typeof(json) == "object"
      && !Js.Array.isArray(json)
      && !((Obj.magic(json): Js.null('a)) === Js.null)) {
    let dict: Js.Dict.t(Js.Json.t) = Obj.magic(json: Js.Json.t);
    switch (Js.Dict.get(dict, key)) {
    | Some(value) =>
      if (value === Aeson_encode.null) {
        None;
      } else {
        Some(decode(value));
      }
    | None => None
    };
  } else {
    raise @@ DecodeError("Expected object, got " ++ Js.Json.stringify(json));
  };

let rec at = (key_path, decoder) =>
  switch (key_path) {
  | [key] => field(key, decoder)
  | [first, ...rest] => field(first, at(rest, decoder))
  | [] =>
    raise @@
    Invalid_argument("Expected key_path to contain at least one element")
  };

let optional = (decode, json) =>
  switch (decode(json)) {
  | exception (DecodeError(_)) => None
  | v => Some(v)
  };

let result = (decodeA, decodeB, json) =>
  switch (Js.Json.decodeObject(json)) {
  | Some(o) =>
    switch (Js_dict.get(o, "Ok")) {
    | Some(l) => Belt.Result.Ok(decodeA(l))
    | None =>
      switch (Js_dict.get(o, "Error")) {
      | Some(r) => Belt.Result.Error(decodeB(r))
      | None =>
        raise @@
        DecodeError(
          "Expected object with a \"Ok\" key or \"Error\" key, got "
          ++ Js.Json.stringify(json),
        )
      }
    }
  | None =>
    raise @@
    DecodeError(
      "Expected object with a \"Ok\" key or \"Error\" key, got "
      ++ Js.Json.stringify(json),
    )
  };

let either = (decodeL, decodeR, json) =>
  switch (Js.Json.decodeObject(json)) {
  | Some(o) =>
    switch (Js_dict.get(o, "Left")) {
    | Some(l) => Aeson_compatibility.Either.Left(decodeL(l))
    | None =>
      switch (Js_dict.get(o, "Right")) {
      | Some(r) => Aeson_compatibility.Either.Right(decodeR(r))
      | None =>
        raise @@
        DecodeError(
          "Expected object with a \"Left\" key or \"Right\" key, got "
          ++ Js.Json.stringify(json),
        )
      }
    }
  | None =>
    raise @@
    DecodeError(
      "Expected object with a \"Left\" key or \"Right\" key, got "
      ++ Js.Json.stringify(json),
    )
  };

let rec oneOf = (decoders, json) =>
  switch (decoders) {
  | [] =>
    let length = List.length(decoders);
    raise @@
    DecodeError(
      {j|Expected oneOf $length, got |j} ++ Js.Json.stringify(json),
    );
  | [decode, ...rest] =>
    switch (decode(json)) {
    | v => v
    | exception _ => oneOf(rest, json)
    }
  };

let tryEither = (a, b) => oneOf([a, b]);

let withDefault = (default, decode, json) =>
  switch (decode(json)) {
  | v => v
  | exception _ => default
  };

let map = (f, decode, json) => f(decode(json));

let andThen = (b, a, json) => b(a(json), json);

let unwrapResult = r =>
  switch (r) {
  | Belt.Result.Ok(v) => v
  | Belt.Result.Error(message) => raise @@ DecodeError(message)
  };

let wrapResult = (decoder, json) =>
  switch (decoder(json)) {
  | v => Belt.Result.Ok(v)
  | exception (DecodeError(message)) => Belt.Result.Error(message)
  };

let int64_of_array = json => {
  let fs = array(float, json);
  if (Array.length(fs) == 2) {
    if (_isInteger(fs[0]) && _isInteger(fs[1])) {
      let left: int32 = Obj.magic(fs[0]: float);
      let right: int32 = Obj.magic(fs[1]: float);
      let res = Int64.of_int32(left);
      let res = Int64.shift_left(res, 32);
      Int64.logor(res, Int64.of_int32(right));
    } else {
      raise @@ DecodeError("Expected int64, got " ++ Js.Json.stringify(json));
    };
  } else {
    raise @@ DecodeError("Expected int64, got " ++ Js.Json.stringify(json));
  };
};

let int64 = json =>
  switch (string(json)) {
  | s => Int64.of_string(s)
  | exception (DecodeError(_)) =>
    switch (string(Js.Json.string(Js.Json.stringify(json)))) {
    | s => Int64.of_string(s)
    | exception (DecodeError(_)) =>
      raise @@
      DecodeError(
        "Expected int64 as string, got " ++ Js.Json.stringify(json),
      )
    }
  };

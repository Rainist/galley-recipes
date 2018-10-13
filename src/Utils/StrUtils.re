let getLeft = (str, idx) => {
  str
    ->String.sub(0, idx);
};

let dropLeft = (str, idx) => {
  let len = str->String.length;
  len->string_of_int;
  str
    ->String.sub(idx+1, len-idx-1);
};

let keyValueOf = line => {
  let rawIndex =
    line
      |> Js.String.indexOf("=");

  let index = switch (rawIndex) {
    | -1 => None
    | x => Some(x)
    };

  switch (index) {
  | Some(n) => {
    let left = line
      -> getLeft(n)
      -> String.trim;
    let right = line
      -> dropLeft(n)
      -> String.trim;
    Some((left, right));
  }
  | None => None
  };
};

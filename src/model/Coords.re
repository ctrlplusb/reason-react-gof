type t = {
  x: int,
  y: int
};

let keyRe = Js.Re.fromString("^x(\\-?[0-9]+)y(\\-?[0-9]+)$");

let fromString = (s: string) =>
  switch (Js.Re.exec(s, keyRe)) {
  | None => None
  | Some(result) =>
    switch (Js.Re.captures(result)) {
    | [|_, x_ns, y_ns|] =>
      switch (Js.Nullable.to_opt(x_ns), Js.Nullable.to_opt(y_ns)) {
      | (Some(x_s), Some(y_s)) =>
        Some({x: int_of_string(x_s), y: int_of_string(y_s)})
      | _ => None
      }
    | _ => None
    }
  };

let neighbours = ({x, y}: t) => [
  /* top left */
  {x: x - 1, y: y - 1},
  /* top above */
  {x, y: y - 1},
  /* top right */
  {x: x + 1, y: y - 1},
  /* left */
  {x: x - 1, y},
  /* right */
  {x: x + 1, y},
  /* bottom left */
  {x: x - 1, y: y + 1},
  /* bottom below */
  {x, y: y + 1},
  /* bottom right */
  {x: x + 1, y: y + 1}
];

let toString = ({x, y}: t) =>
  "x" ++ string_of_int(x) ++ "y" ++ string_of_int(y);
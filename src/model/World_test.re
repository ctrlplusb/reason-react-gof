open Jest;

open Expect;

open Coords;

open World;

let draw = world => {
  let (result, _) =
    World.fold_left(
      ((drawing, prevY), ({x, y}, state)) => (
        drawing
        ++ (
          switch prevY {
          | None => ""
          | Some(y_) => y_ != y ? "\n" : ""
          }
        )
        ++ (
          switch state {
          | Alive => "x"
          | _ => "-"
          }
        ),
        Some(y)
      ),
      ("", None),
      world
    );
  result;
};

let strip = (s: string) =>
  Js.String.replaceByRe([%bs.re "/ /g"], "", s) |> Js.String.trim;

let expectWorld = (world, expected) => {
  let actual = world |> draw |> strip;
  expect(actual) |> toEqual(expected |> strip);
};

describe("World", () => {
  test("create 1x1 without seed", () =>
    expectWorld(World.create(~cols=1, ~rows=1, [||]), {|
      -
    |})
  );
  test("create 2x2 without seed", () =>
    expectWorld(
      World.create(~cols=2, ~rows=2, [||]),
      {|
      --
      --
      |}
    )
  );
  test("create 2x3 without seed", () =>
    expectWorld(
      World.create(~cols=3, ~rows=2, [||]),
      {|
      ---
      ---
      |}
    )
  );
  test("create 2x3 with seed", () =>
    expectWorld(
      World.create(
        ~cols=3,
        ~rows=2,
        [|{x: 2, y: 0}, {x: 0, y: 1}, {x: 1, y: 1}|]
      ),
      {|
        --x
        xx-
      |}
    )
  );
  describe("step", () => {
    let expectStepsToEqual = (~steps, expected) => {
      let world =
        World.create(
          ~cols=4,
          ~rows=3,
          [|{x: 2, y: 0}, {x: 0, y: 1}, {x: 1, y: 1}, {x: 2, y: 2}|]
        );
      Array.init(steps, x => x) |> Array.iter((_) => World.step(world));
      test(string_of_int(steps) ++ " steps", () =>
        expectWorld(world, expected)
      );
    };
    expectStepsToEqual(~steps=0, {|
      --x-
      xx--
      --x-
    |});
    expectStepsToEqual(~steps=1, {|
      -x--
      -xx-
      -x--
    |});
    expectStepsToEqual(~steps=2, {|
      -xx-
      xxx-
      -xx-
    |});
    expectStepsToEqual(~steps=3, {|
      x-x-
      x--x
      x-x-
    |});
  });
});
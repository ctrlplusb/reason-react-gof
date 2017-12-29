/**
 * Abstraction type for Matrix functor allowing specification of
 * the type of value that each address within the matrix can hold.
 */
module type Value = {type t;};

/**
 * A functor to produce a simple, mutable, matrix data structure that
 * can represent a theoretically infinite matrix.
 */
module Make = (Value: Value) => {
  type v = Value.t;
  type t = Js.Dict.t(option(v));
  /**
   * Creates an instance.
   */
  let create = () => Js.Dict.empty();
  /**
   * Gets the value at the given coordinates.
   */
  let get = (~coords, matrix) =>
    switch (Js.Dict.get(matrix, Coords.toString(coords))) {
    | None => None
    | Some(x) => x
    };
  /**
   * Gets all the values for neighbours of the given coordinate.
   * i.e. the coordinates that surround the target coordinate within the matrix.
   */
  let neighbours = (~coords, world) =>
    List.fold_left(
      (acc, coords) => [get(~coords, world), ...acc],
      [],
      Coords.neighbours(coords)
    );
  /**
   * Sets a value at the given coordinate.
   */
  let set = (~coords, ~value, world) =>
    Js.Dict.set(world, Coords.toString(coords), Some(value));
  /**
   * Unsets a value at the given coordinate.
   */
  let unset = (~coords, world) =>
    Js.Dict.set(world, Coords.toString(coords), None);
};
type state =
  | Alive
  | Dead;

/**
 * Module to transport our "cell" type into the MakeMatrix functor.
 */
module Cell = {
  type t = state;
};

/**
 * The matrix data type which will be used as the data store.
 */
module WorldMatrix = Matrix.Make(Cell);

/**
 * Use the WorldMatrix type as the primary i/o type for our
 * module.
 */
type t = {
  rows: int,
  cols: int,
  matrix: WorldMatrix.t
};

let incrementAlive = (acc, cur) =>
  switch cur {
  | Some(Alive) => acc + 1
  | _ => acc
  };

let countLive = List.fold_left(incrementAlive, 0);

let applyRules = (current, neighbours) =>
  switch (current, neighbours) {
  | (Alive, 2)
  | (Alive, 3)
  | (Dead, 3) => Alive
  | _ => Dead
  };

let fold_left = (fn, acc, world) => {
  let rows = Array.init(world.rows, x => x);
  let cols = Array.init(world.cols, x => x);
  Array.fold_left(
    (accRow, row) =>
      Array.fold_left(
        (accCol, col) => {
          let coords: Coords.t = {x: col, y: row};
          let state =
            switch (WorldMatrix.get(~coords, world.matrix)) {
            | Some(Alive) => Alive
            | _ => Dead
            };
          fn(accCol, (coords, state));
        },
        accRow,
        cols
      ),
    acc,
    rows
  );
};

let create = (~cols, ~rows, seed) => {
  let matrix = WorldMatrix.create();
  Array.iter(coords => WorldMatrix.set(~coords, ~value=Alive, matrix), seed);
  {rows, cols, matrix};
};

let step = world =>
  world
  |> fold_left(
       (acc, (coords, state)) => {
         let neighbours = WorldMatrix.neighbours(~coords, world.matrix);
         let alive = countLive(neighbours);
         let newState = applyRules(state, alive);
         [(coords, newState), ...acc];
       },
       []
     )
  |> List.iter(((coords, value)) =>
       WorldMatrix.set(~coords, ~value, world.matrix)
     );

let to_list = world =>
  fold_left((acc, current) => [current, ...acc], [], world);
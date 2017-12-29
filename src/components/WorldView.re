[%bs.raw {|require('./worldView.css')|}];

module Grid = {
  let cellSize = 100;
  type state = {world: option(World.t)};
  type action =
    | Initialize(World.t);
  let createWorld = (width, height) =>
    World.create(
      ~rows=height / cellSize + 2,
      ~cols=width / cellSize + 2,
      [||]
    );
  let component = ReasonReact.reducerComponent("Grid");
  let make = (~width, ~height, _children) => {
    ...component,
    initialState: () => {world: None},
    didMount: ({reduce}) => {
      reduce(() => Initialize(createWorld(width, height)), ());
      ReasonReact.NoUpdate;
    },
    reducer: (action, _) =>
      switch action {
      | Initialize(world) => ReasonReact.Update({world: Some(world)})
      },
    render: ({state: {world}}) =>
      switch world {
      | None => ReasonReact.nullElement
      | Some(world) =>
        <div className="WorldView">
          (
            ReasonReact.arrayToElement(
              World.fold_left(
                (rows, ({y}, state)) => {
                  let rowKey = string_of_int(y);
                  let row = Js.Dict.get(rows, rowKey);
                  let cell =
                    <span className="WorldView-cell">
                      (
                        ReasonReact.stringToElement(
                          switch state {
                          | Alive => "A"
                          | Dead => "D"
                          }
                        )
                      )
                    </span>;
                  switch row {
                  | None =>
                    Js.Dict.set(rows, rowKey, [cell]);
                    rows;
                  | Some(row) =>
                    Js.Dict.set(rows, rowKey, [cell, ...row]);
                    rows;
                  };
                },
                Js.Dict.empty(),
                world
              )
              |> Js.Dict.entries
              |> Array.map(((row, cells)) =>
                   <div key=row className="WorldView-row">
                     (ReasonReact.arrayToElement(Array.of_list(cells)))
                   </div>
                 )
            )
          )
        </div>
      }
  };
};

let component = ReasonReact.statelessComponent("WorldView");

let make = _children => {
  ...component,
  render: (_) =>
    <ViewPortSize render=(({width, height}) => <Grid width height />) />
};
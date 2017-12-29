/**
 * This component uses the "render" prop pattern to provide the window
 * dimensions to the given render function.
 *
 * @example
 *   <ViewPortSize
 *     render=(({ width, height }) =>
 *       <div>
 *         (ReasonReact.stringToElement("w" ++ string_of_int(width) ++ " h" ++ string_of_int(height)))
 *       </div>
 *     )
 *   />
 *
 * @todo
 *  - throttling (configurable with default?)
 */
type dimensions = {
  width: int,
  height: int
};

type onResizeHandler = ReasonReact.Callback.t(Dom.event);

type state = {
  dimensions: option(dimensions),
  onWindowResizeHandler: option(onResizeHandler)
};

type action =
  | InitialLoad(option(onResizeHandler), option(dimensions))
  | WindowResized(option(dimensions));

let getWindowDimensions = () =>
  Some({
    height: WindowRe.innerHeight(Webapi.Dom.window),
    width: WindowRe.innerWidth(Webapi.Dom.window)
  });

let component = ReasonReact.reducerComponent("ViewPortSize");

let make = (~render, _children) => {
  ...component,
  initialState: () => {dimensions: None, onWindowResizeHandler: None},
  didMount: ({reduce}) => {
    let onWindowResizeHandler =
      reduce(_event => WindowResized(getWindowDimensions()));
    WindowRe.addEventListener(
      "resize",
      onWindowResizeHandler,
      Webapi.Dom.window
    );
    reduce(
      () => InitialLoad(Some(onWindowResizeHandler), getWindowDimensions()),
      ()
    );
    ReasonReact.NoUpdate;
  },
  willUnmount: ({state: {onWindowResizeHandler}}) => {
    let unmounter =
      switch onWindowResizeHandler {
      | Some(handler) => (
          () =>
            WindowRe.removeEventListener("resize", handler, Webapi.Dom.window)
        )
      | _ => (() => ())
      };
    unmounter();
  },
  reducer: (action, state) =>
    switch action {
    | InitialLoad(onWindowResizeHandler, dimensions) =>
      ReasonReact.Update({onWindowResizeHandler, dimensions})
    | WindowResized(dimensions) => ReasonReact.Update({...state, dimensions})
    },
  render: ({state: {dimensions}}) =>
    switch dimensions {
    | None => ReasonReact.nullElement
    | Some(dimensions) => render(dimensions)
    }
};
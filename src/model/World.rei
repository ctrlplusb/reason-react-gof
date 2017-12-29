/**
 * Game of Life World
 *
 * The universe of the Game of Life is a two-dimensional orthogonal grid of
 * square cells, each of which is in one of two possible states, alive
 * or dead.
 *
 * Every cell interacts with its eight neighbours, which are the cells that are
 * horizontally, vertically, or diagonally adjacent. At each step in time, the
 * following transitions occur:
 *
 * - Any live cell with fewer than two live neighbours dies, as if caused by
 *   underpopulation.
 *
 * - Any live cell with two or three live neighbours lives on to the next
 *   generation.
 *
 * - Any live cell with more than three live neighbours dies, as if by
 *   overpopulation.
 *
 * - Any dead cell with exactly three live neighbours becomes a live cell, as
 *   if by reproduction.
 *
 * - The initial pattern constitutes the seed of the system. The first
 *   generation is created by applying the above rules simultaneously to every
 *   cell in the seed—births and deaths occur simultaneously, and the discrete
 *   moment at which this happens is sometimes called a tick (in other words,
 *   each generation is a pure function of the preceding one). The rules
 *   continue to be applied repeatedly to create further generations.
 */
/**
 * The world.
 */
type t;

/**
 * Represents the state of a cell within the world.
 */
type state =
  | Alive
  | Dead;

/**
 * Creates a new world confined to within the given size and initialized
 * with the given seed.
 */
let create: (~cols: int, ~rows: int, array(Coords.t)) => t;

/**
 * Provides a mechanism to fold over each cell within the world. From left to
 * right and top to bottom.
 */
let fold_left: (('acc, (Coords.t, state)) => 'acc, 'acc, t) => 'acc;

/**
 * Evolves the given world, applying the Game of Life rules.
 */
let step: t => unit;

/**
 * Gets a list representation of the world boundary.
 */
let to_list: t => list((Coords.t, state));
export default abstract class Sketch {
  canvas: HTMLCanvasElement;
  ctx: CanvasRenderingContext2D;

  constructor(canvas: HTMLCanvasElement) {
    this.canvas = canvas;
    this.ctx = this.canvas.getContext("2d")!;
  }

  /**
   * Run once for initialization
   */
  abstract setup(): void;

  /**
   * Draws sketch, run repeatedly
   */
  abstract draw(): void;

  /**
   * Update sketch, run repeatedly
   */
  abstract update(): void;

  /**
   * Creates event listeners for user input
   */
  abstract createEventListeners(): void;

  /**
   * Call this method to run the sketch
   */
  run() {
    this.setup();
    this.createEventListeners();
    const drawFunc = () => {
      requestAnimationFrame(drawFunc);
      this.update();
      this.draw();
    };
    drawFunc();
  }
}


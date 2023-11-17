export default abstract class Sketch {
  canvas: HTMLCanvasElement;
  ctx: CanvasRenderingContext2D;
  fps: number;
  _lastFrame: number;

  constructor(canvas: HTMLCanvasElement) {
    this.canvas = canvas;
    this.ctx = this.canvas.getContext("2d")!;
    this.fps = 60;
    this._lastFrame = 0;
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
      const curTime = Date.now();
      if (curTime >= this._lastFrame + 1000/this.fps){
          this.update();
          this._lastFrame = curTime;
      }
      this.draw();
    };
    drawFunc();
  }
}


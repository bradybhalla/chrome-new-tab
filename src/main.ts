import "./style.css";

type Rect = {
  left: number,
  top: number,
  width: number,
  height: number;
};

abstract class Sketch {
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

class MainSketch extends Sketch {
  mouse = { x: 0, y: 0, pressed: false, visible: false };
  insideSearch = false;

  searchBoxEl = document.getElementById("search-box")!;
  searchEl: HTMLInputElement = document.getElementById("search")! as HTMLInputElement;
  searchFocused: boolean = false;

  searchBoxRect: Rect = { left: 0, top: 0, width: 0, height: 0 };

  setSearchBoxPos(): void {
    const w = (this.canvas.width * 0.7 < 500 ? 500 : this.canvas.width * 0.7);

    this.searchBoxRect = {
      left: (this.canvas.width - w) / 2,
      top: this.canvas.height * 0.25 - 50,
      width: w,
      height: 50
    };

    this.searchBoxEl.style.left = this.searchBoxRect.left + "px";
    this.searchBoxEl.style.top = this.searchBoxRect.top + "px";
    this.searchBoxEl.style.width = this.searchBoxRect.width + "px";
    this.searchBoxEl.style.height = this.searchBoxRect.height + "px";
  }

  drawRandomRect(left: number, top: number, width: number, height: number) {
    const w = width/4;
    const h = height/4;
    const l = Math.random() * (width - w) + left;
    const t = Math.random() * (height - h) + top;
    this.ctx.fillStyle = "#111111";
    this.ctx.fillRect(l, t, w, h);
  }

  setup(): void {
    this.canvas.width = window.innerWidth;
    this.canvas.height = window.innerHeight;
    this.setSearchBoxPos();
  }
  draw(): void {

    this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);

    if (this.searchFocused) {
      for (let i = 0; i < 40; i++) {
         this.drawRandomRect(this.searchBoxRect.left - 15, this.searchBoxRect.top - 15, this.searchBoxRect.width + 30, this.searchBoxRect.height + 30);
      }
      this.ctx.clearRect(this.searchBoxRect.left, this.searchBoxRect.top, this.searchBoxRect.width, this.searchBoxRect.height);
    }

    if (this.mouse.visible) {
      this.ctx.fillStyle = this.insideSearch ? "#202124" : "lightgray";
      if (this.insideSearch) {
        this.ctx.fillRect(this.mouse.x - 1, this.mouse.y - 24 / 2, 2, 24);
      } else {
        this.ctx.beginPath();
        this.ctx.ellipse(this.mouse.x, this.mouse.y, 4, 4, 0, 0, Math.PI * 2);
        this.ctx.fill();
        this.ctx.closePath();
      }
    }
  }
  update(): void {
    this.insideSearch = this.mouse.x > this.searchBoxRect.left &&
      this.mouse.x < this.searchBoxRect.left + this.searchBoxRect.width &&
      this.mouse.y > this.searchBoxRect.top &&
      this.mouse.y < this.searchBoxRect.top + this.searchBoxRect.height;
    this.searchFocused = document.activeElement == this.searchEl;
  }
  createEventListeners(): void {

    window.addEventListener("resize", () => {
      this.canvas.width = window.innerWidth;
      this.canvas.height = window.innerHeight;
      this.mouse.visible = false;
      this.setSearchBoxPos();
      this.draw();
    });

    addEventListener("keydown", (e) => {
      if (e.key == "Enter" && this.searchFocused) {
        window.location.href = "https://www.google.com/search?q=" + encodeURIComponent(this.searchEl.value);
      } else if (e.key == "Enter" && !this.searchFocused){
        this.searchEl.focus();
      } else if (e.key == "Escape") {
        this.searchEl.blur();
      }
    });

    addEventListener("mousedown", (e) => {
      this.mouse.pressed = true;
      if (this.insideSearch) {
        this.searchEl.focus();
        e.preventDefault();
      }
    });

    addEventListener("mouseup", () => {
      this.mouse.pressed = false;
    });


    addEventListener("mousemove", (e) => {
      this.mouse.visible = true;
      this.mouse.x = e.clientX;
      this.mouse.y = e.clientY;
    });

    addEventListener("mouseout", () => {
      this.mouse.visible = false;
    });
  }

}

(new MainSketch(<HTMLCanvasElement>document.getElementById("sketch"))).run();

import Sketch from "./sketch";
import SandManager from "./sand";

import "./style.css";

type Rect = {
  left: number,
  top: number,
  width: number,
  height: number
};

class MainSketch extends Sketch {
  sandSize = 0;
  
  frameCounter = 0;

  sand: SandManager | null = null;

  mouse = { x: 0, y: 0, pressed: false, visible: false };
  insideSearch = false;

  searchBoxEl = document.getElementById("search-box")!;
  searchEl: HTMLInputElement = document.getElementById("search")! as HTMLInputElement;
  searchFocused: boolean = false;

  searchBoxRect: Rect = { left: 0, top: 0, width: 0, height: 0 };

  coordToGrid(x: number, y: number) {
    return { col: Math.floor(x / this.sandSize), row: Math.floor(y / this.sandSize) };
  }

  setSearchBoxPos(): void {
    const w = (this.canvas.width * 0.5 < 500 ? 500 : this.canvas.width * 0.5);

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

  override setup(): void {
    this.canvas.width = window.innerWidth;
    this.canvas.height = window.innerHeight;
    this.setSearchBoxPos();

    this.sandSize = Math.max(Math.floor(this.canvas.height / 100), 7);
    this.sand = new SandManager(Math.floor(this.canvas.height / this.sandSize + 0.5), Math.floor(this.canvas.width / this.sandSize + 0.5));
  }
  override draw(): void {

    this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);

    // draw sand
    if (this.sand != null && this.sand.sandExists) {
      this.ctx.fillStyle = "#666666";
      for (let row = 0; row < this.sand.rows; row++) {
        for (let col = 0; col < this.sand.cols; col++) {
          if (this.sand.sands[row][col]) {
            this.ctx.fillRect(col * this.sandSize, row * this.sandSize, this.sandSize + 1, this.sandSize + 1);
          }
        }
      }
    }

    this.ctx.clearRect(this.searchBoxRect.left, this.searchBoxRect.top, this.searchBoxRect.width, this.searchBoxRect.height);

    // draw mouse
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
  override update(): void {
    this.frameCounter++;
    this.insideSearch = this.mouse.x > this.searchBoxRect.left &&
      this.mouse.x < this.searchBoxRect.left + this.searchBoxRect.width &&
      this.mouse.y > this.searchBoxRect.top &&
      this.mouse.y < this.searchBoxRect.top + this.searchBoxRect.height;
    this.searchFocused = document.activeElement == this.searchEl;
    
    if (this.mouse.pressed && this.mouse.visible && !this.insideSearch) {
      this.sand?.addAt(this.coordToGrid(this.mouse.x, this.mouse.y));
    }

    this.sand?.update();
    this.sand?.update();
  }
  override createEventListeners(): void {

    this.searchEl.addEventListener("focus", () => {
      let corner1 = this.coordToGrid(this.searchBoxRect.left, this.searchBoxRect.top + this.searchBoxRect.height);
      let corner2 = this.coordToGrid(this.searchBoxRect.left + this.searchBoxRect.width, this.searchBoxRect.top + this.searchBoxRect.height);
      this.sand?.addInColRange(
        corner1.row,
        corner1.col,
        corner2.col
      );
    });

    addEventListener("resize", () => {
      this.canvas.width = window.innerWidth;
      this.canvas.height = window.innerHeight;
      this.mouse.visible = false;
      this.setSearchBoxPos();
      this.sandSize = Math.max(Math.floor(this.canvas.height / 100), 7);
      this.sand?.extend(Math.floor(this.canvas.height / this.sandSize + 0.5), Math.floor(this.canvas.width / this.sandSize + 0.5));
      this.draw();
    });

    addEventListener("keydown", (e) => {
      if (e.key == "Enter" && this.searchFocused && this.searchEl.value != "") {
        window.location.href = "https://www.google.com/search?q=" + encodeURIComponent(this.searchEl.value);
      } else if (e.key == "Enter" && !this.searchFocused) {
        this.searchEl.focus();
      } else if (e.key == "Escape") {
        this.searchEl.blur();
      } else if (e.key == "Backspace") {
        this.sand?.removeSome();
      } else if (e.key == "Tab" || e.key == "Shift" || e.key == "Meta" || e.key == "Control" || e.key == "Alt") {

      } else if (this.searchFocused) {
        this.sand?.addBlocks();
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

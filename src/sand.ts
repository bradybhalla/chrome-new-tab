export default class SandManager {
  rows: number;
  cols: number;

  sands: boolean[][];

  sandExists: boolean;

  constructor(_rows: number, _cols: number) {
    this.rows = _rows;
    this.cols = _cols;

    this.sands = new Array(this.rows);
    for (let i = 0; i < this.rows; i++) {
      this.sands[i] = new Array(this.cols).fill(false);
    }

    this.sandExists = false;
  }

  isSand(row: number, col: number) {
    return !(row >= 0 && row < this.rows && col >= 0 && col < this.cols) || this.sands[row][col];
  }

  extend(totalRows: number, totalCols: number): void {
    const newSands: boolean[][] = new Array(totalRows);
    for (let i = 0; i < totalRows; i++) {
      newSands[i] = new Array(totalCols).fill(false);
    }

    const growRow = totalRows >= this.rows;
    const growCol = totalCols >= this.cols;
    const colAdder = Math.floor(Math.abs(this.cols - totalCols) / 2 + Math.random());
    for (let row = 0; row < (growRow ? this.rows : totalRows); row++) {
      for (let col = 0; col < (growCol ? this.cols : totalCols); col++) {
        newSands[row][col + (growCol ? colAdder : 0)] = this.sands[row + (growRow ? 0 : this.rows - totalRows)][col + (growCol ? 0 : colAdder)];
      }
    }

    this.rows = totalRows;
    this.cols = totalCols;
    this.sands = newSands;
  }

  addBlocks() {
    const num = Math.floor(Math.random() * 5) + 5;
    for (let i = 0; i < num; i++) {
      const size = Math.floor(Math.random() * 5) + 7;
      const col = Math.floor(Math.random() * (this.cols - size + 1));
      for (let r = 0; r < size; r++) {
        for (let c = 0; c < size; c++) {
          this.sands[r][col + c] = true;
        }
      }
    }

    this.sandExists = true;
  }

  addAt({row, col}: {row: number, col: number}){
    for (let r = row-1; r <= row+1; r++) {
      for (let c = col-1; c <= col+1; c++) {
        if (!this.isSand(r,c) && Math.random() < 0.5){
          this.sands[r][c] = true;
        }
      }
    }
    this.sandExists = true;
  }

  addInColRange(row: number, colStart: number, colEnd: number){
    for (let r = row; r < row+10; r++) {
      for (let c = colStart; c <= colEnd; c++) {
        if (!this.isSand(r,c) && Math.random() < 1-(r-row)*0.1){
          this.sands[r][c] = true;
        }
      }
    }
    this.sandExists = true;
  }

  removeSome() {
    for (let r = 0; r < this.rows; r++) {
      for (let c = 0; c < this.cols; c++) {
        if (Math.random() < 0.3){
          this.sands[r][c] = false;
        }
      }
    }
  }

  update() {
    if (!this.sandExists) {
      return;
    }
    for (let row = this.rows - 1; row >= 0; row--) {
      const scanLeft = Math.random() < 0.5;

      for (let c = 0; c < this.cols; c++) {
        const col = scanLeft ? c : this.cols - 1 - c;
        if (this.isSand(row, col)) {
          const left = this.isSand(row + 1, col - 1);
          const center = this.isSand(row + 1, col);
          const right = this.isSand(row + 1, col + 1);

          if (left && right && center) {
            continue;
          } else if (!center) {
            this.sands[row + 1][col] = true;
            this.sands[row][col] = false;
          } else if (!left && !right) {
            if (Math.random() < 0.5) {
              this.sands[row + 1][col - 1] = true;
              this.sands[row][col] = false;
            } else {
              this.sands[row + 1][col + 1] = true;
              this.sands[row][col] = false;
            }
          } else if (!left) {
            this.sands[row + 1][col - 1] = true;
            this.sands[row][col] = false;
          } else if (!right) {
            this.sands[row + 1][col + 1] = true;
            this.sands[row][col] = false;
          }
          
        }
      }
    }
  }
}
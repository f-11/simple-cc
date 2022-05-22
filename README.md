# simple-cc
Cコンパイラになりかけのなにかです。[低レイヤを知りたい人のためのCコンパイラ作成入門](https://github.com/f-11/simple-cc)に基づいています。SIMPLEアーキテクチャ（の元祖）については[こちら](https://isle3hw.kuis.kyoto-u.ac.jp/simple_v40.pdf)に書いてありますが、ISAは多少追加されています。

## 使用例
  git clone https://github.com/f-11/simple-cc.git
  cd simple-cc
  make
  ./simple-cc "int main() { return 0; }" > sample.txt

sample.txtをアセンブルして実機にロードしてあげてください。

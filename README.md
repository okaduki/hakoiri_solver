# hakoiri_solver

[箱入り娘](https://ja.wikipedia.org/wiki/%E7%AE%B1%E5%85%A5%E3%82%8A%E5%A8%98_%E3%83%91%E3%82%BA%E3%83%AB)のソルバ。

## ビルド

```bash
make
```

## 実行方法

```bash
./hakoiri_solver < musume1.txt
```

## 入力形式

`musume1.txt`が一般的な(?)初期配置の例なので見てください。
0が空きマス、1が娘で他は同じ文字が一塊のコマです。
ゴールは初期配置の0の位置です。

## その他

雑に書いたので終わるまでに2,3分程度かかります。
メモリは3GBぐらい使ってました。
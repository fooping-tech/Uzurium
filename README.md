# Uzurium

Uzurium は、エモーショナルな渦を生成するオープンソースの装置です。
音楽・演劇・アートなどの表現活動において、独創的な演出を可能にすることを目指しています。

![Uzurium_pic](https://user-images.githubusercontent.com/4471301/230387553-9a522a33-7e51-4807-b8a6-9094ee4bfcd9.png)

## 特徴

- **オープンソース**：ソフトウェア・ハードウェアともに公開しており、自由に改良・再利用できます。
- **多彩な表現**：モータの回転と LED の光を組み合わせて、独自の渦を生み出せます。
- **作りやすさ**：シンプルな構造で、部品も入手しやすくなっています。
- **協調制御**：ESP-NOW で無線接続し、複数台の Uzurium を連携させられます。

## しくみ

DC モータで磁石を回し、瓶の中の回転子へ磁力で動力を伝えて渦を発生させます。
電源は、据え置きに便利な USB と、持ち運びに便利な電池の両方に対応しています。

![内部構造](https://user-images.githubusercontent.com/4471301/230392971-7a4f7620-4429-47e3-91af-170f551910af.png)

### 主な構成部品

- M5Atom（または M5Stamp）
- 制御基板（モータドライバ・昇圧電源・フォトリフレクタ・LED 18 灯を搭載）
- FA-130 モータ
- ネオジム磁石・回転子・瓶
- 電池ボックス
- 専用ケース（3D プリント）

## リポジトリ構成

```
Uzurium/
├── src/                      ファームウェア（Arduino スケッチ）
│   ├── Uzurium/              本体用（M5Atom）
│   ├── UzuriumController/    コントローラ用（M5Stack Core2）
│   └── UzuriumMaster/        マスター用（M5Stack Core2）
├── hardware/
│   ├── pcb/                  基板設計データ
│   │   ├── UzuriumCircuit_2/       最新基板：Gerber・BOM・レイアウト図・DXF
│   │   └── UzuriumBoard_v0.0.0/    初代基板：回路図・変更箇所
│   └── stl/                  3D プリント用データ
│       ├── common/           共通部品（プロペラ・モータブラケット等）
│       ├── case/             専用ケース
│       └── devkit/           DevKit 用部品
└── docs/
    └── BuildManual.md        DevKit 組立説明書
```

## ハードウェア

| 内容 | 場所 |
|---|---|
| 基板（最新） | [hardware/pcb/UzuriumCircuit_2](hardware/pcb/UzuriumCircuit_2) — Gerber・部品表・レイアウト図・外形 DXF |
| 基板（初代 v0.0.0） | [hardware/pcb/UzuriumBoard_v0.0.0](hardware/pcb/UzuriumBoard_v0.0.0) — 回路図と実装時の定数変更 |
| 3D プリント部品 | [hardware/stl](hardware/stl) |

## ビルドガイド

組み立て方法は [DevKit 組立説明書](docs/BuildManual.md) を参照してください。
はんだ付けなど、基本的な電子工作のスキルが必要です。

## ファームウェアの書き込み

Arduino IDE と M5Stack のボードマネージャがインストール済みであることを前提とします。

### 1. プロジェクトのダウンロード

```sh
git clone https://github.com/fooping-tech/Uzurium.git
```

または GitHub の「Code → Download ZIP」からダウンロードしてください。

### 2. ライブラリのインストール

Arduino IDE のライブラリマネージャから、使用するスケッチに応じて以下をインストールしてください。

| スケッチ | 対象デバイス | 必要なライブラリ |
|---|---|---|
| `src/Uzurium` | M5Atom | M5Atom, FastLED, arduinoFFT |
| `src/UzuriumController` | M5Stack Core2 | M5Unified, arduinoFFT, ArduinoTrace |
| `src/UzuriumMaster` | M5Stack Core2 | M5Core2 |

### 3. 書き込み

Uzurium 本体の場合は `src/Uzurium/Uzurium.ino` を Arduino IDE で開き、M5Atom に書き込みます。
コントローラ・マスターも同様に、各フォルダの `.ino` ファイルを開いて書き込んでください。

## ライセンス

Uzurium は MIT ライセンスで公開しています。詳細は [LICENSE](LICENSE) を参照してください。

## Third-party software

This firmware uses arduinoFFT.

- arduinoFFT version: 1.6.2
- License: GNU GPL v3
- Source: https://github.com/kosme/arduinoFFT

The source code corresponding to the firmware distributed with the device
is available in this repository.


## 作者

Uzurium は [@FoopingTech](https://github.com/fooping-tech) が開発しています。
ご意見・ご要望は Issues または Pull Request でお寄せください。

## 貢献方法

バグ報告、ドキュメントやコードの改善、新機能の追加など、Uzurium の改善・発展へのご協力を歓迎します。
詳細は [CONTRIBUTING.md](CONTRIBUTING.md) を参照してください。

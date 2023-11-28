# Uzurium
Uzuriumは、エモーショナルな渦を生成するオープンソースの装置です。この装置は、音楽や演劇、アートなどの表現活動において、独創的な演出を可能にすることが期待されています。
 
![Uzurium_pic](https://user-images.githubusercontent.com/4471301/230387553-9a522a33-7e51-4807-b8a6-9094ee4bfcd9.png)


# 特徴

オープンソースソフトウェア、オープンソースハードウェアであるため、自由に改良・再利用が可能です。
独自の渦を生成することができ、多様な表現が可能です。
シンプルな構造であり、部品を調達しやすいため、製作が容易です。

# システム構成

Uzuriumは、以下の部品から構成されています。

- FA-130モータ
- NEO PIXEL LED RING 18
- ネオジム磁石
- 電池BOX
- M5Atom or M5Stamp
- モータドライバ
- 専用ケース
- フォトリフレクタ
- 制御基板
- 回転子
- 瓶

![内部構造](https://user-images.githubusercontent.com/4471301/230392971-7a4f7620-4429-47e3-91af-170f551910af.png)

DCモータで磁石を回し、瓶の中の回転子へ動力を伝え渦を発生させます。 電源は据え置きに便利なUSBと持ち運びに便利な電池をサポート。 ESP-NOWで無線接続しており協調制御を実現。


# ビルドガイド

Uzuriumの制作方法については、[こちら](https://github.com/fooping-tech/Uzurium/blob/main/docs/BuildManual.md)のページに詳細が記載されています。
制作にあたっては、基本的な電子工作のスキルが必要です。


# 回路図

[Uzurium Board v0.0.0](https://github.com/fooping-tech/Uzurium/blob/main/schematics/SCH_UzuriumCircuit_2023-08-30.pdf)

# Getting Started ガイド

このガイドでは、プロジェクトのセットアップから始め、基本的な操作について説明します。プロジェクトをダウンロードして実行するための手順を追います。
Arduino IDEインストール済み、M5Stackのボードマネージャがインストール済みであることを前提に記載します。

## 目次

1. [プロジェクトのダウンロード](#1-プロジェクトのダウンロード)
2. [ライブラリのインストール](#2-ライブラリのインストール)
3. [プログラムの書き込み](#3-プログラムの書き込み)

## 1. プロジェクトのダウンロード

まず、プロジェクトをローカルにダウンロードしてください。

## 2. ライブラリのインストール

Arduino IDEのライブラリマネージャから以下のライブラリをインストールしてください。
- M5Atom
- FastLED
- arduinoFFT

## 3. プログラムの書き込み
ダウンロードしたフォルダの中のUzurium/src/Uzurium.inoをダブルクリックしてファイルを開きM5ATOMへ書き込みます。


# ライセンス

Uzuriumは、MITライセンスのもとで公開されています。詳細は、LICENSEファイルを参照してください。
また以下を使用しています。
 [FastLED](https://github.com/FastLED/FastLED/blob/master/LICENSE)
 
# 作者

Uzuriumは、@FoopingTechによって開発されました。ご意見・ご要望などは、IssuesまたはPull Requestにてお寄せください。

# 貢献方法

Uzuriumの改善・発展にご協力いただける方は、以下の方法で貢献してください。
詳細は[CONTRIBUTING](https://github.com/fooping-tech/Uzurium/blob/0c022eda2cfa34b28f84850665d2f155f35a8e37/CONTRIBUTING.md)を参照してください。

- バグ報告
- ドキュメントの改善
- コードの改善
- 新しい機能の追加



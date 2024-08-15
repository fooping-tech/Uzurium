class Button {
private:
    int pin;
    int cur_value;
    int last_value;

public:
    // コンストラクタでピン番号を設定
    Button(int buttonPin) : pin(buttonPin), cur_value(0), last_value(0) {}

    // ボタンの初期設定
    void setup() {
        pinMode(pin, INPUT_PULLUP);
    }

    // ボタンの状態をチェック
    bool check() {
        bool checkBtn = false;
        cur_value = digitalRead(pin);
        if (cur_value != last_value) {
            if (cur_value == 0) {
                checkBtn = true;
            } else {
                checkBtn = false;
            }
        }
        last_value = cur_value;
        return checkBtn;
    }
};
# include <Siv3D.hpp>
#include"HandObject.h"
#include"Shots.h"
#include"Targets.h"
#include"Cannon.h"

double absolute(const double x) {
    return x >= 0 ? x : -x;
}

enum Mode { Shooting, Changing, Announce };

//-1:間違えたヒット　0:ヒットなし　1:ヒット
int HitCheak(Shots& S, Targets& T, Result mode) {
    const int r = S.ShotSize + T.TargetSize;
    auto Titr = T.Targets_.begin();
    for (auto& Shot : S.bullets) {
        if (Shot.state >= Hit) {
            continue;
        }
        while (Titr != T.Targets_.end()) {
            if (Shot.position.y <= (*Titr).position.y + r && absolute(Shot.position.x - (*Titr).position.x) < r) {
                Result result = JunkenResult(static_cast<Hand>(Shot.state), static_cast<Hand>((*Titr).state));
                Shot.state = Hit;
                T.Targets_.erase(Titr);
                if (result == mode) {
                    return 1;
                }
                else {
                    return -1;
                }
            }
            else {
                ++Titr;
            }
        }
    }
    return 0;
}




// シーンの名前
enum class State
{
    Title,
    Game
};

// ゲームデータ
struct GameData
{
    // ハイスコア
    int32 highScore = 0;
};

// シーン管理クラス
using MyApp = SceneManager<State, GameData>;

// タイトルシーン
class Title : public MyApp::Scene
{
private:

    Rect m_startButton = Rect(Arg::center = Scene::Center().movedBy(0, 0), 300, 60);
    Transition m_startTransition = Transition(0.4s, 0.2s);

    Rect m_exitButton = Rect(Arg::center = Scene::Center().movedBy(0, 100), 300, 60);
    Transition m_exitTransition = Transition(0.4s, 0.2s);

public:

    Title(const InitData& init)
        : IScene(init) {}

    void update() override
    {
        m_startTransition.update(m_startButton.mouseOver());
        m_exitTransition.update(m_exitButton.mouseOver());

        if (m_startButton.mouseOver() || m_exitButton.mouseOver())
        {
            Cursor::RequestStyle(CursorStyle::Hand);
        }

        if (m_startButton.leftClicked())
        {
            changeScene(State::Game);
        }

        if (m_exitButton.leftClicked())
        {
            System::Exit();
        }
    }

    void draw() const override
    {
        const String titleText1 = U"ジャンケン";
        const String titleText2 = U"シューティング";
        const Vec2 center1(Scene::Center().x, 120);
        const Vec2 center2(Scene::Center().x, 220);
        FontAsset(U"Title")(titleText1).drawAt(center1.movedBy(4, 6), ColorF(0.0, 0.5));
        FontAsset(U"Title")(titleText1).drawAt(center1);
        FontAsset(U"Title")(titleText2).drawAt(center2.movedBy(4, 6), ColorF(0.0, 0.5));
        FontAsset(U"Title")(titleText2).drawAt(center2);

        m_startButton.draw(ColorF(1.0, m_startTransition.value())).drawFrame(2);
        m_exitButton.draw(ColorF(1.0, m_exitTransition.value())).drawFrame(2);

        FontAsset(U"Menu")(U"はじめる").drawAt(m_startButton.center(), ColorF(0.25));
        FontAsset(U"Menu")(U"おわる").drawAt(m_exitButton.center(), ColorF(0.25));

        Rect(0, 500, Scene::Width(), Scene::Height() - 500)
            .draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

        const int32 highScore = getData().highScore;
        FontAsset(U"Score")(U"High score: {}"_fmt(highScore)).drawAt(Vec2(300, 550));
    }
};

// ゲームシーン
class Game : public MyApp::Scene
{
private:
    // スコア
    int32 m_score = 0;

    int HP = 10;
    Targets targets;
    Cannon cannon;
    Result rule = Win;
    Mode mode = Announce;
    const double Announce_time = 2.0;
    double timer = 0.0;
    int count = 30;
public:

    Game(const InitData& init)
        : IScene(init)
    {
        targets.stop_add();
    }

    void update() override
    {
        if (targets.LandingCheack()) {
            --HP;
        }
        if (targets.update()) {
            --count;
        }
        cannon.update();

        int rc = HitCheak(cannon.shots, targets, rule);
        if (rc == -1) {
            --HP;
        }
        else if (rc == 1) {
            m_score += rc;
        }

        if (HP <= 0) {
            changeScene(State::Title);
            getData().highScore = Max(getData().highScore, m_score);
        }

        if (mode == Shooting && count <= 0) {
            timer = 0.0;
            targets.stop_add();
            mode = Changing;
            count = 30;
        }
        else if (mode == Changing && targets.isempty()) {
            mode = Announce;
            rule = static_cast<Result>((Random<int>(1, 2) + static_cast<int>(rule)) % 3);
        }
        else if (mode == Announce) {
            timer += Scene::DeltaTime();
            if (timer >= Announce_time) {
                mode = Shooting;
                targets.restart_add();
            }
        }
    }

    void draw() const override
    {
        targets.draw();
        cannon.draw();
        FontAsset(U"Score")(U"スコア：{}"_fmt(m_score)).drawAt(Scene::Width() * 3 / 4, 25);
        FontAsset(U"Score")(U"耐久：{}"_fmt(HP)).drawAt(Scene::Width() * 3 / 4, 60);
        if (mode == Announce) {
            if (rule == Win) FontAsset(U"Announce")(U"勝て！").drawAt(Scene::Center());
            else if (rule == Lose) FontAsset(U"Announce")(U"負けろ！").drawAt(Scene::Center());
            else if (rule == Draw) FontAsset(U"Announce")(U"引き分けろ！").drawAt(Scene::Center());
        }
    }
};

void Main()
{
    //スクリーンサイズを
    Window::Resize(Size(700, 700));

    Window::SetTitle(U"ジャンケンシューティング");

    // 使用するフォントアセットを登録
    FontAsset::Register(U"Title", 100, Typeface::Bold);
    FontAsset::Register(U"Menu", 30, Typeface::Regular);
    FontAsset::Register(U"Score", 30, Typeface::Bold);
    FontAsset::Register(U"Announce", 80, Typeface::Bold);

    TextureAsset::Register(U"RockTexture", Emoji(U"✊🏻"), AssetParameter::LoadImmediately());
    TextureAsset::Register(U"PaperTexture", Emoji(U"🖐🏻"), AssetParameter::LoadImmediately());
    TextureAsset::Register(U"ScissorsTexture", Emoji(U"✌"), AssetParameter::LoadImmediately());
    TextureAsset::Register(U"ExplosionTexture", Emoji(U"💥"), AssetParameter::LoadImmediately());
    TextureAsset::Register(U"CannonTexture", Emoji(U"👃🏿"), AssetParameter::LoadImmediately());
    // 背景色を設定
    Scene::SetBackground(ColorF(0.2, 0.8, 0.4));

    // シーンと遷移時の色を設定
    MyApp manager;
    manager
        .add<Title>(State::Title)
        .add<Game>(State::Game)
        .setFadeColor(ColorF(1.0));

    while (System::Update())
    {
        if (!manager.update())
        {
            break;
        }
    }
}


//
// = アドバイス =
// Debug ビルドではプログラムの最適化がオフになります。
// 実行速度が遅いと感じた場合は Release ビルドを試しましょう。
// アプリをリリースするときにも、Release ビルドにするのを忘れないように！
//
// 思ったように動作しない場合は「デバッグの開始」でプログラムを実行すると、
// 出力ウィンドウに詳細なログが表示されるので、エラーの原因を見つけやすくなります。
//
// = お役立ちリンク =
//
// OpenSiv3D リファレンス
// https://siv3d.github.io/ja-jp/
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/basic/
//
// よくある間違い
// https://siv3d.github.io/ja-jp/articles/mistakes/
//
// サポートについて
// https://siv3d.github.io/ja-jp/support/support/
//
// Siv3D ユーザコミュニティ Slack への参加
// https://siv3d.github.io/ja-jp/community/community/
//
// 新機能の提案やバグの報告
// https://github.com/Siv3D/OpenSiv3D/issues
//

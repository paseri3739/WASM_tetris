#ifndef B8AC84C6_6A5A_4990_8095_F03C1115A0EC
#define B8AC84C6_6A5A_4990_8095_F03C1115A0EC

#include <core/Position.hpp>
#include <core/graphics_types.hpp>  // Color, Rect など
#include <string>
#include <tl/expected.hpp>

/**
 * @file IRenderer.hpp
 * @brief バックエンド非依存のレンダリングインターフェース定義
 *
 * このインターフェースを実装することで、SDL2・OpenGL・Vulkan など
 * 異なるレンダリングバックエンドに対して同一の API を通じて描画処理を行えるようにする。
 * 主に以下の機能を提供する：
 *   - フレーム制御（begin_frame/end_frame）
 *   - 画面クリア
 *   - プリミティブ描画（線分・矩形・点など）
 *   - バッチ描画（複数の点・線・矩形を一度に描画）
 *   - テクスチャの登録・描画・描画ターゲット切り替え
 *   - フォント登録・テキスト描画・文字列キャッシュ
 *   - 論理解像度（logical size）・ビューポート・スケール設定
 *
 * 以下に各メソッドの詳細と使用例を記載しているので、実装時の参考にしてください。
 */

using TextureId = std::uint32_t;  ///< テクスチャ識別子
using FontId = std::uint32_t;     ///< フォント識別子

/**
 * @class IRenderer
 * @brief レンダリング処理をバックエンド非依存で抽象化したインターフェース
 *
 * ゲームやアプリケーションの描画処理を実装する際に、このインターフェースを継承した具象クラス（例：SDLRenderer）
 * を作成し、各種描画メソッドをバックエンド固有の処理で実装することで、「描画コードの上位層を一切変更せずに」
 * バックエンドを切り替えられるようにする。
 *
 * 主な機能群：
 *   - フレーム制御：begin_frame(), end_frame()
 *   - クリア処理：clear(Color)
 *   - プリミティブ描画：fill_rect, stroke_rect, draw_line, draw_point
 *   - バッチ描画：draw_points, draw_lines, fill_rects
 *   - テクスチャ管理：register_texture_from_file, register_texture_from_memory, set_render_target
 *   - テクスチャ描画：draw_texture
 *   - フォント・テキスト描画：register_font, draw_text, measure_text, create_text_texture
 *   - 論理解像度・ビューポート・スケール：set_logical_size, set_viewport, set_scale
 *
 * @note
 *   - Color は RGBA の 4 要素を持つ構造体を想定（例：{r, g, b, a}）。各種描画メソッドで使用する。
 *   - Rect は矩形領域を示す構造体 (pos: Position, size: {width, height})
 * を想定。ピクセル単位で座標・サイズを指定。
 *   - Position は 2D 座標 (x, y) を示す構造体を想定。
 *   - tl::expected<T, std::string> は、T が成功時の返却値、std::string
 * が失敗時のエラーメッセージを表す。
 */
class IRenderer {
   public:
    virtual ~IRenderer() = default;

    //=========================================================================
    // 1. フレーム制御
    //=========================================================================
    /**
     * @brief フレーム開始時の処理を行う
     *
     * フレームごとに begin_frame() → 各種描画コール → end_frame() の順で呼び出す。
     * バックエンドによっては、GUI ライブラリとの連携や内部ステートリセットを行う場合がある。
     *
     * @note 実装が不要であれば空メソッドでもよいが、将来的に ImGui
     * などを挿入する場合を想定しておく。
     */
    virtual void begin_frame() = 0;

    /**
     * @brief フレーム終了時の処理を行う
     *
     * 通常はフロントバッファへのスワップ（SDL2 の SDL_RenderPresent、OpenGL の SwapBuffers
     * など）を行う。 end_frame() を呼ばないと画面に描画内容が反映されない。
     */
    virtual void end_frame() = 0;

    //=========================================================================
    // 2. 画面クリア
    //=========================================================================
    /**
     * @brief 画面全体を指定色でクリアする
     * @param color クリア時の色 (RGBA)。Alpha は無視されるバックエンドもある。
     *
     * 例）黒でクリアしたい場合：
     *   renderer->clear({ 0, 0, 0, 255 });
     */
    virtual void clear(Color color) = 0;

    //=========================================================================
    // 3. プリミティブ描画（単体）
    //=========================================================================
    /**
     * @brief 矩形を塗りつぶして描画する
     * @param rect  描画矩形 (左上座標 + サイズ)
     * @param color 塗りつぶし色 (RGBA)
     *
     * 例）幅 50、高さ 30 の赤い長方形を (100,100) に描画：
     *   Rect r = { {100, 100}, {50, 30} };
     *   renderer->fill_rect(r, {255, 0, 0, 255});
     */
    virtual void fill_rect(const Rect& rect, Color color) = 0;

    /**
     * @brief 矩形の枠線のみを描画する
     * @param rect  描画矩形 (左上座標 + サイズ)
     * @param color 枠線の色 (RGBA)
     *
     * 例）幅 80、高さ 40 の青い枠を (200,150) に描画：
     *   Rect r = { {200, 150}, {80, 40} };
     *   renderer->stroke_rect(r, {0, 0, 255, 255});
     */
    virtual void stroke_rect(const Rect& rect, Color color) = 0;

    /**
     * @brief 2 点間を結ぶ線分を描画する
     * @param start 線分の始点座標 (x, y)
     * @param end   線分の終点座標 (x, y)
     * @param color 線分の色 (RGBA)
     *
     * 例）座標 (50,50) から (150,80) へ緑の線を描画：
     *   renderer->draw_line({50, 50}, {150, 80}, {0, 255, 0, 255});
     */
    virtual void draw_line(Position start, Position end, Color color) = 0;

    /**
     * @brief 指定位置に 1 ピクセルを描画する
     * @param pos   描画位置の座標 (x, y)
     * @param color 点の色 (RGBA)
     *
     * 例）座標 (10,10) に白いピクセルを描画：
     *   renderer->draw_point({10, 10}, {255, 255, 255, 255});
     */
    virtual void draw_point(const Position& pos, Color color) = 0;

    //=========================================================================
    // 4. プリミティブ描画（バッチ）
    //=========================================================================
    /**
     * @brief 複数の点を一度にバッチ描画する
     * @param points 描画する座標の配列 (Position[])。配列長は count で指定。
     * @param count  描画する点の数
     * @param color  全点に共通の色 (RGBA)
     *
     * 例）3 点をまとめて黄色で描画：
     *   Position pts[3] = { {10,10}, {20,20}, {30,15} };
     *   renderer->draw_points(pts, 3, {255, 255, 0, 255});
     */
    virtual void draw_points(const Position* points, int count, Color color) = 0;

    /**
     * @brief 複数の線分を一度にバッチ描画する
     * @param segments 線分の座標配列 (Position[]){ start0, end0, start1, end1, … }
     * @param count    描画する線分の数
     * @param color    全線分に共通の色 (RGBA)
     *
     * 例）2 本の線分を赤で描画：
     *   Position segs[4] = { {10,10}, {50,10}, {60,20}, {80,40} };
     *   renderer->draw_lines(segs, 2, {255, 0, 0, 255});
     */
    virtual void draw_lines(const Position* segments, int count, Color color) = 0;

    /**
     * @brief 複数の塗りつぶし矩形を一度にバッチ描画する
     * @param rects 描画する矩形配列 (Rect[])
     * @param count 配列の要素数
     * @param color 全矩形に共通の色 (RGBA)
     *
     * 例）3 つの矩形をまとめて描画：
     *   Rect rects[3] = {
     *       { {10,10}, {20,20} },
     *       { {40,40}, {30,10} },
     *       { {70,20}, {10,30} }
     *   };
     *   renderer->fill_rects(rects, 3, {0, 128, 255, 255});
     */
    virtual void fill_rects(const Rect* rects, int count, Color color) = 0;

    //=========================================================================
    // 5. テクスチャ描画
    //=========================================================================
    /**
     * @brief 登録済みテクスチャを矩形単位で描画する
     * @param id          登録時に取得した TextureId
     * @param src_region  テクスチャ上の切り出し元矩形 (UV 座標のピクセル矩形)
     *                    例：{ {0,0}, {32,32} } なら左上 32×32 の範囲を切り出す
     * @param dst_region  画面上の描画先矩形 (左上座標 + サイズ)。拡大縮小を行いたい場合、src.size ≠
     * dst.size とする
     * @param angle       回転角（度数法）。テクスチャを回転して描画したい場合に指定。回転中心は
     * dst_region の中央を想定
     *
     * @note
     *   - (pivot, flip, color_mod) を別途追加して実装してもよい。    *   -
     * テクスチャが不透明ピクセル／透明ピクセル混在の場合はアルファ合成に注意する（set_blend_mode
     * を先に設定するなど）。
     *
     * 例）36×36 のアイコンテクスチャから中央 32×32 部分を切り出して、(100,100) に 64×64
     * で回転なしで描画： TextureId icon = …;  // 事前に register_texture_from_file で読み込み済み
     *   Rect src  = { {2,2}, {32,32} };
     *   Rect dst  = { {100,100}, {64,64} };
     *   renderer->draw_texture(icon, src, dst, 0.0);
     */
    virtual void draw_texture(TextureId id, const Rect& src_region, const Rect& dst_region,
                              double angle) = 0;

    //=========================================================================
    // 6. フォント登録・テキスト描画
    //=========================================================================
    /**
     * @brief フォントファイルをロードして登録する
     * @param path    フォントファイルのパス（TTF/OTF などを想定）
     * @param pt_size ポイントサイズ（例：24）
     * @return
     *   - 成功: 登録済みフォントを示す FontId    *   - 失敗: エラーメッセージ (例："TTF_OpenFont
     * failed: …")
     *
     * @note
     *   - フォント登録後は draw_text, measure_text, create_text_texture などで使用できる。    *   -
     * 実装側で TTF_Init() が済んでいない場合はエラーとなるので、create()
     * 等で必ず初期化しておくこと。
     *
     * 例）32pt のフォントをロード：
     *   auto res = renderer->register_font("assets/fonts/MyFont.ttf", 32);
     *   if (!res) {
     *       std::cerr << res.error() << std::endl;
     *   }
     *   FontId font = res.value();
     */
    [[nodiscard]]
    virtual tl::expected<FontId, std::string> register_font(const std::string& path,
                                                            int pt_size) = 0;

    [[nodiscard]]
    virtual tl::expected<FontId, std::string> clear_font(FontId id) = 0;

    /**
     * @brief 文字列を即時描画する（キャッシュなし）
     * @param font_id 登録済みの FontId
     * @param utf8    UTF-8 形式の文字列（例："Hello, 世界!"）
     * @param pos     描画左上座標 (x, y)
     * @param color   テキストカラー (RGBA)
     * @return
     *   - 成功: 空 (tl::expected<void, string>::value() を返す)    *   - 失敗: エラーメッセージ
     * (例："TTF_RenderUTF8_Blended failed: …")
     *
     * @note
     *   - 実装側では TTF_RenderUTF8_Blended → SDL_CreateTextureFromSurface → SDL_RenderCopy →
     * SDL_DestroyTexture の順で、一時的にテクスチャ化して描画する。    *   -
     * 毎フレーム同じ文字列を描画する場合は、create_text_texture
     * を使ってテクスチャ化し、draw_texture で再利用すると高速。
     *
     * 例）(50,200) に白文字でテキストを描画：
     *   auto err = renderer->draw_text(font, u8"スコア: 1000", {50, 200}, {255,255,255,255});
     *   if (!err) {
     *       std::cerr << err.error() << std::endl;
     *   }
     */
    [[nodiscard]]
    virtual tl::expected<void, std::string> draw_text(FontId font_id, const std::string& utf8,
                                                      Position pos, Color color) = 0;

    /**
     * @brief フォントで指定した文字列 (UTF-8) の描画サイズを計測する
     * @param font_id 登録済みの FontId
     * @param utf8    UTF-8 形式の文字列
     * @return (幅, 高さ) のペアを返す (単位: ピクセル)。失敗時は (0,0) を返す。
     *
     * 例）"Hello" のサイズを取得：
     *   auto [w, h] = renderer->measure_text(font, "Hello");
     *   // w, h に描画に必要な幅・高さが入る
     */
    virtual std::pair<int, int> measure_text(FontId font_id, const std::string& utf8) = 0;

    /**
     * @brief 文字列をテクスチャ化してキャッシュ登録する
     * @param font_id 登録済みの FontId
     * @param utf8    UTF-8 形式の文字列
     * @param color   テキストカラー (RGBA)
     * @return
     *   - 成功: テクスチャを示す TextureId    *   - 失敗: エラーメッセージ
     * (例："TTF_RenderUTF8_Blended failed: …")
     *
     * @note
     *   - create_text_texture で生成されたテクスチャは、ユーザーが明示的に破棄するまで
     *     (または IRenderer のデストラクタで一括破棄されるまで) メモリ上に保持される。    *   -
     * 毎フレーム同じ文字列を描画する場合は、draw_text よりもこちらを使い、 draw_texture
     * で再利用することで高速化できる。
     *
     * 例）"Game Over" をテクスチャ化し、(100,50) に描画：
     *   auto tex_res = renderer->create_text_texture(font, u8"Game Over", {255, 0, 0, 255});
     *   if (!tex_res) {
     *       std::cerr << tex_res.error() << std::endl;
     *   }
     *   TextureId text_tex = tex_res.value();
     *   Rect src = { {0,0}, {200, 50} };  // テクスチャ幅・高さは measure_text などで取得しておく
     *   Rect dst = { {100,50}, {200, 50} };
     *   renderer->draw_texture(text_tex, src, dst, 0.0);
     */
    virtual tl::expected<TextureId, std::string> create_text_texture(FontId font_id,
                                                                     const std::string& utf8,
                                                                     Color color) = 0;

    //=========================================================================
    // 7. 論理解像度・ビューポート・スケール設定
    //=========================================================================
    /**
     * @brief 論理解像度 (logical size) を設定する
     * @param width  内部座標系の幅 (例: 320)
     * @param height 内部座標系の高さ (例: 240)
     *
     * @note
     *   - これ以降のすべての描画コールは width×height の座標系で行われ、
     *     実際のウィンドウサイズに合わせて自動的に拡大縮小される。    *   - 例えばウィンドウが
     * 1280×960 の場合、320×240 で描画したものを 4 倍拡大して表示する。    *   -
     * ドット絵をジャギーのまま整数倍拡大したい場合に必須。
     *
     * 例）論理解像度を 320×240 に固定：
     *   renderer->set_logical_size(320, 240);
     */
    virtual void set_logical_size(int width, int height) = 0;

    /**
     * @brief ビューポート (描画可能領域) を指定する
     * @param clip_rect クリップ矩形 (左上座標 + サイズ)。描画はこの領域のみに制限される。
     *
     * @note
     *   - 論理解像度とは独立に動作する場合があるため、呼び出しタイミングに注意する。
     *   - UI のクリッピングや分割スクリーン描画、ミニマップ表示などで利用する。
     *
     * 例）左半分のみ描画可能に設定 (x=0,y=0,width=160,height=240)：
     *   renderer->set_viewport({ {0,0}, {160,240} });
     */
    virtual void set_viewport(const Rect& clip_rect) = 0;

    /**
     * @brief スケーリング比を指定する
     * @param scale_x X 方向の倍率 (例: 2.0f)
     * @param scale_y Y 方向の倍率 (例: 2.0f)
     *
     * @note
     *   - set_logical_size と組み合わせることで、任意の拡大縮小比を設定できる。    *   -
     * 例：logical size=320×240、scale=2.0×2.0 → 実際には 640×480 で描画する。
     *
     * 例）X2 倍、Y3 倍で拡大：
     *   renderer->set_scale(2.0, 3.0);
     */
    virtual void set_scale(double scale_x, double scale_y) = 0;

    //=========================================================================
    // 8. テクスチャ登録・描画ターゲット切り替え
    //=========================================================================
    /**
     * @brief ファイルパスからテクスチャを生成・登録する
     * @param path  画像ファイルのパス (PNG/JPEG 等を想定)
     * @return
     *   - 成功: TextureId（登録済みテクスチャを一意に指す）    *   - 失敗: エラーメッセージ
     * (例："IMG_Load failed: …" / "SDL_CreateTextureFromSurface failed: …")
     *
     * @note
     *   - 実装側では SDL_image (IMG_Load) → SDL_CreateTextureFromSurface → register_texture
     * の流れを想定。    *   - テクスチャは内部マップに登録され、ID を通じて描画・更新・破棄が可能。
     *
     * 例）"assets/sprite.png" を読み込んでテクスチャを用意：
     *   auto res = renderer->register_texture_from_file("assets/sprite.png");
     *   if (!res) std::cerr << res.error() << std::endl;
     *   TextureId sprite = res.value();
     */
    [[nodiscard]]
    virtual tl::expected<TextureId, std::string> register_texture_from_file(
        const std::string& path) = 0;

    /**
     * @brief 生データ (RGBA8) からテクスチャを生成・登録する
     * @param width      テクスチャ幅 (ピクセル)
     * @param height     テクスチャ高さ (ピクセル)
     * @param pixel_data 生データバッファ (size = width * height * 4 バイトを想定)
     * @return
     *   - 成功: TextureId    *   - 失敗: エラーメッセージ (例："SDL_CreateTexture failed: …" /
     * "SDL_UpdateTexture failed: …")
     *
     * @note
     *   - ランタイムで画像処理した結果を直接テクスチャ化したい場合に使用。    *   - SDL2 実装では
     * SDL_CreateTexture → SDL_UpdateTexture → register_texture の流れを想定。
     *
     * 例）128×128 の RGBA ピクセルデータをテクスチャ化：
     *   std::vector<unsigned char> data(128 * 128 * 4);
     *   // data を自前で埋める…
     *   auto res = renderer->register_texture_from_memory(128, 128, data.data());
     *   TextureId dynamicTex = res.value();
     */
    [[nodiscard]]
    virtual tl::expected<TextureId, std::string> register_texture_from_memory(
        int width, int height, const unsigned char* pixel_data) = 0;

    /**
     * @brief 描画ターゲットを切り替える (Render Target)
     * @param tex_id
     *   - tex_id == 0: ウィンドウ（デフォルトレンダーターゲット）に戻す    *   - tex_id != 0:
     * register_texture_* で登録済みのテクスチャ ID を指定して切り替え
     * @return
     *   - 成功: 空
     *   - 失敗: エラーメッセージ (例："set_render_target: invalid texture id" /
     * "SDL_SetRenderTarget failed: …")
     *
     * @note
     *   - 仮想画面を作成し、オフスクリーンレンダーターゲットに描画してから
     *     最後にウィンドウに貼り付ける「論理解像度固定パターン」などで使用する。
     *   - OpenGL/Vulkan 実装では glBindFramebuffer 等に対応した処理を行う想定。
     *
     * 例）テクスチャ ID 5 をレンダーターゲットに設定：
     *   auto err = renderer->set_render_target(5);
     *   if (!err) std::cerr << err.error() << std::endl;
     *   // 以降の描画はすべて tex_id=5 のテクスチャに対して行われる
     *   // ...
     *   // 元のウィンドウに戻す
     *   renderer->set_render_target(0);
     */
    virtual tl::expected<void, std::string> set_render_target(TextureId tex_id) = 0;
};

//========= Usage Example (使用例) ===============================================
//
// 以下は IRenderer を利用した典型的なレンダリングループのサンプルです。
// 実際には SDLRenderer や OpenGLRenderer など、IRenderer を継承した具象クラスを生成して使用します。
//
// // 1. IRenderer の生成例（SDLRenderer を想定）
// std::unique_ptr<SDL_Window> window( SDL_CreateWindow(
//     "Sample", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//     800, 600, SDL_WINDOW_SHOWN ) );
// auto renderer_res = SDLRenderer::create(window.get(),
//     SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
// if (!renderer_res) {
//     std::cerr << "Renderer 作成失敗: " << renderer_res.error() << std::endl;
//     std::exit(1);
// }
// std::unique_ptr<IRenderer> renderer = std::move(renderer_res.value());
//
// // 2. リソース登録例
// // フォント登録
// auto font_res = renderer->register_font("assets/fonts/MyFont.ttf", 24);
// if (!font_res) {
//     std::cerr << font_res.error() << std::endl;
// }
// FontId mainFont = font_res.value();
//
// // テクスチャ登録 (ファイルから)
// auto tex_res = renderer->register_texture_from_file("assets/images/sprite.png");
// if (!tex_res) {
//     std::cerr << tex_res.error() << std::endl;
// }
// TextureId spriteTex = tex_res.value();
//
// // 動的テクスチャ登録 (生データから)
// // std::vector<unsigned char> pixelData(width*height*4);
// // auto dyn_res = renderer->register_texture_from_memory(width, height, pixelData.data());
// // TextureId dynTex = dyn_res.value();
//
// // 3. 論理解像度設定 (320x240) + nearest フィルタ化 (ドット絵用)
// // (SDL2 実装なら、事前に SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"nearest") を呼ぶ)
// renderer->set_logical_size(320, 240);
//
// // 4. メインループ
// bool running = true;
// while (running) {
//     // （入力処理など）
//     // ...
//
//     // フレーム開始
//     renderer->begin_frame();
//
//     // 画面クリア (黒)
//     renderer->clear({0, 0, 0, 255});
//
//     // プリミティブ描画例：赤い 50x50 の四角を (10,10) に描画
//     renderer->fill_rect({ {10, 10}, {50, 50} }, {255, 0, 0, 255});
//
//     // テクスチャ描画例：spriteTex の左上 32x32 部分を (100,100) に 64x64 で描画
//     Rect src  = { {0,0}, {32,32} };
//     Rect dst  = { {100,100}, {64,64} };
//     renderer->draw_texture(spriteTex, src, dst, 0.0);
//
//     // テキスト即時描画例：スコアを表示
//     renderer->draw_text(mainFont, u8"Score: 1234", {10, 200}, {255, 255, 255, 255});
//
//     // テキストキャッシュ描画例：同じ文字列を何度も描画するなら下記を事前に実行
//     // auto txt_tex_res = renderer->create_text_texture(mainFont, u8"Level: 5", {0, 255, 0,
//     255});
//     // TextureId txtTex = txt_tex_res.value();
//     // Rect txtSrc = { {0,0}, {64,16} };
//     // Rect txtDst = { {200,10}, {64,16} };
//     // renderer->draw_texture(txtTex, txtSrc, txtDst, 0.0);
//
//     // フレーム終了
//     renderer->end_frame();
//
//     // ウィンドウイベント処理（SDL2 の場合）
//     SDL_Event e;
//     while (SDL_PollEvent(&e)) {
//         if (e.type == SDL_QUIT) {
//             running = false;
//         }
//     }
// }
//
// // 5. 後片付けは各オブジェクトのデストラクタで自動的に行われる
// //    ウィンドウ破棄、SDL_DestroyRenderer、TTF_Quit、SDL_Quit など
//
//==============================================================================

#endif /* B8AC84C6_6A5A_4990_8095_F03C1115A0EC */

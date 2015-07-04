nt main(int argc, char* argv[]) {
    // ファイル入力ストリームの初期化
    std::ifstream ifs("C:\\home\\himitsu.txt");

    std::string line;
    while (std::getline(ifs, line)) {
        // ファイルの中身を一行づつ表示
        std::cout << line << std::endl;
    }

    // ファイル出力ストリームの初期化
    std::ofstream ofs("C:\\home\\nazo.txt");

    // ファイルに1行ずつ書き込み
    ofs << "ham" << std::endl;
    ofs << "egg" << std::endl;
    ofs << "spam" << std::endl;

    return 0;
}

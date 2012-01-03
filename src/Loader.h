struct StageData;
struct LevelData;

class Loader
{
public:
	static const StageData LoadStage();
	static const LevelData LoadLevel(int stage, int level);
};


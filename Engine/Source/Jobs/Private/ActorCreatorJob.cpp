#include "Jobs\ActorCreatorJob.h"

namespace engine {
namespace jobs {

ActorCreatorJob::ActorCreatorJob(const engine::util::FileUtils::FileData& i_file_data, const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback) : file_data_(i_file_data),
	callback_(i_callback)
{
	// validate inputs
	ASSERT(i_file_data.file_contents);
	ASSERT(i_file_data.file_size > 0);
	ASSERT(i_callback);
}

ActorCreatorJob::~ActorCreatorJob()
{}

void ActorCreatorJob::DoWork()
{
	engine::memory::SharedPointer<engine::gameobject::Actor> actor;
	engine::gameobject::ActorCreator::CreateActorFromFileData(file_data_, actor);
	callback_(actor);
}

} // namespace jobs
} // namespace engine

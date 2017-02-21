#ifndef ACTOR_CREATOR_JOB_H_
#define ACTOR_CREATOR_JOB_H_

// library includes
#include <functional>

// engine includes
#include "GameObject\Actor.h"
#include "GameObject\ActorCreator.h"
#include "Jobs\InterfaceJob.h"
#include "Memory\SharedPointer.h"
#include "Util\FileUtils.h"

namespace engine {
namespace jobs {

class ActorCreatorJob : public InterfaceJob
{
public:
	ActorCreatorJob(const engine::util::FileUtils::FileData& i_file_data, const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback);
	~ActorCreatorJob();

	// implement InterfaceJob
	void DoWork() override;

	// accessors and mutators
	inline const engine::util::FileUtils::FileData& GetFileData() const;
	inline void SetFileData(const engine::util::FileUtils::FileData& i_file_data);

	inline const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& GetCallback() const;
	inline void SetCallback(const std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>& i_callback);

private:
	ActorCreatorJob(const ActorCreatorJob&) = delete;
	ActorCreatorJob(ActorCreatorJob&&) = delete;

	ActorCreatorJob& operator=(const ActorCreatorJob&) = delete;
	ActorCreatorJob& operator=(ActorCreatorJob&&) = delete;

	engine::util::FileUtils::FileData																file_data_;
	std::function<void(engine::memory::SharedPointer<engine::gameobject::Actor>)>					callback_;

}; // class ActorCreatorJob

} // namespace jobs
} // namespace engine

#include "ActorCreatorJob-inl.h"

#endif // ACTOR_CREATOR_JOB_H_

#include "OOOUnitTestDefines.h"
#include "TestRunnerApplication.h"

OOOTest(TestRunnerApplication)
{
	/* Post a quit message before starting the app (as it will otherwise not return from the constructor) */
	o_message tMessage;
	tMessage.msg_class = MSG_CLASS_CONTROL;
	tMessage.type = MSG_TYPE_QUIT;
	assert(O_post_message(&tMessage) == GOOD);

	{
		TestRunnerApplication * pTestRunnerApplication = OOOConstruct(TestRunnerApplication);
		OOOCall(pTestRunnerApplication, start);
		OOODestroy(pTestRunnerApplication);
	}
}

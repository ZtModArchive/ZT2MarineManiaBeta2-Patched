-- mantaray.lua

include "scenario/scripts/ui.lua";
include "scenario/scripts/entity.lua";
include "scenario/scripts/economy.lua";
include "scenario/scripts/misc.lua";

CHALLENGE_NAME = "mantaray";
ANIMAL = "RayManta_Adult"
CHALLENGE_REWARD_FAME = 1.875 -- one tick

function validate()

	if (isChalDeclinedForever(CHALLENGE_NAME) == true) then
		return 0;
	end
	
	-- Set the current challenge in a global variable for debugging purposes
	setglobalvar("current_challenge_name", CHALLENGE_NAME);
	
	local mgr = queryObject("BFScenarioMgr");
	if (mgr) then
		-- If we are gathering prereqs, output the challenge name
		if (getPrereqGather() == true) then
			BFLOG(SYSTRACE, CHALLENGE_NAME);
		end
				
		-- Increment the number of challenges which have been offered
		incChallengesOffered();
			
		-- Only give the scenario if we are not suppressing them
		if (getAddScenarioSuppression() == false) then
			mgr:BFS_ADDSCENARIO("scenario/goals/challenge/mantaray.xml");
		elseif (getPrereqGather() == false) then
			logDebugChallengeInfo(CHALLENGE_NAME, "skipped");
		end
				
		return 1;
	end
		
	return 0;
end


function evalMantaRay(comp)

	challenge = getglobalvar("challenge");
	
	if (challenge == "accept") then
		local mgr = queryObject("BFScenarioMgr");	
		if (mgr) then
			mgr:BFS_SHOWRULE(CHALLENGE_NAME);
		end
		
		
		logDebugChallengeInfo(CHALLENGE_NAME, "accepted");
		
		setchallengeactive();
		
		comp.accept = 1;
		showprimarygoals();
	elseif (challenge == "decline") then
	
		if (getDeclineForever() == true) then
			setglobalvar(CHALLENGE_NAME.."_decline_forever", "true");
			
			logDebugChallengeInfo(CHALLENGE_NAME, "declinedforever");
			
			-- If we make it here we want to reset decline forever
			setDeclineForever(false);
		elseif (getChallengeDebugMode() == true) then
			logDebugChallengeInfo(CHALLENGE_NAME, "declined");
		end
	
		declinenotfail = 1;
		
		-- Return failure
		return -1;
	end
	
	if (comp.accept == nil) then
		if (challenge == nil) then
		
			showinstantchallengepanel("Challengetext:CHMantaRay");
			setglobalvar("challenge", "waiting")
		end
	end
	
	-- Check to see if we have reach Goal
	if (comp.accept == 1) then
		if(thisManyExist(ANIMAL,3)) then
			return 1
		end
		
		return checkTimeLimit(comp,1,0)
			
		
	end	
	
	
	return 0;
end

function completeMantaRay(comp)	
	-- give 5 guests and a tick of zoo fame
	BFLOG("Current Fame level: "..getZooFame())
	setMaxFame(getZooFame() + CHALLENGE_REWARD_FAME)
	BFLOG("New Fame level(one tick higher): "..getZooFame())
	giveGuest(5);
	showchallengewin("Challengetext:CHMantaRaySuccess");
	
	resetchallengeoverandcomplete(CHALLENGE_NAME);
	incrementglobalchallenges();
end

function failMantaRay(comp)
	if (declinenotfail == nil) then
		showchallengefail("Challengetext:CHMantaRayFailure");
	end
	
	resetchallengeover(CHALLENGE_NAME);
	declinenotfail = nil;
end
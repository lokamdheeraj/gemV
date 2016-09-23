/*
 * Copyright (c) 2012-2014 ARM Limited
 * All rights reserved
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Andrew Bardsley
 */

#include "cpu/minor/stats.hh"

namespace Minor
{

	MinorStats::MinorStats()
	{ }

	void
		MinorStats::regStats(const std::string &name, BaseCPU &baseCpu)
		{
			numInsts
				.name(name + ".committedInsts")
				.desc("Number of instructions committed");

			numOps
				.name(name + ".committedOps")
				.desc("Number of ops (including micro ops) committed");

			numDiscardedOps
				.name(name + ".discardedOps")
				.desc("Number of ops (including micro ops) which were discarded "
						"before commit");

			numFetchSuspends
				.name(name + ".numFetchSuspends")
				.desc("Number of times Execute suspended instruction fetching");

			quiesceCycles
				.name(name + ".quiesceCycles")
				.desc("Total number of cycles that CPU has spent quiesced or waiting "
						"for an interrupt")
				.prereq(quiesceCycles);

			cpi
				.name(name + ".cpi")
				.desc("CPI: cycles per instruction")
				.precision(6);
			cpi = baseCpu.numCycles / numInsts;

			ipc
				.name(name + ".ipc")
				.desc("IPC: instructions per cycle")
				.precision(6);
			ipc = numInsts / baseCpu.numCycles;

			/// vulnerability calculators

			PCvul
				.name(name + ".PCvul")
				.desc("Vulnerability of PC, in terms of tick");
			// PCvul = PCvul * 32;

			transferQueueInFetchVul
				.name(name + ".transferQueueInFetchVul")
				.desc("Vulnerability of transfers queue in fetch1 unit, in terms of tick");
			// PCvul = PCvul * 32;
			requestsQueueInFetchVul
				.name(name + ".requestsQueueInFetchVul")
				.desc("Vulnerability of requets queue in fetch1 unit, in terms of tick");
			// PCvul = PCvul * 32;

			fetch2InputBufferVul
				.name(name + ".fetch2InputBufferVul")
				.desc("Vulnerability of inputBuffer in fetch2 unit, in terms of tick");
			decInputBufferVul
				.name(name + ".decInputBufferVul")
				.desc("Vulnerability of inputBuffer in Decode unit, in terms of tick");

			exeInputBufferVul
				.name(name + ".exeInputBufferVul")
				.desc("Vulnerability of inputBuffer in execute unit, in terms of tick");
			lsqTransQueVul
				.name(name + ".lsqTransQueVul")
				.desc("Vulnerability of Transfers Queue in LSQ, in terms of tick");
			lsqReqsQueVul
				.name(name + ".lsqReqsQueVul")
				.desc("Vulnerability of Requests Queue in LSQ, in terms of tick");
			lsqStBufVul
				.name(name + ".lsqStBufVul")
				.desc("Vulnerability of Store Buffer in LSQ, in terms of tick");
/*
			LSQVul
				.name(name + ".LSQVul")
				.desc("Vulnerability of Load-Store queue, in terms of tick");
*/


sbnumResultsVul
				.name(name + ".sbVul.sbnumResultsVul")
				.desc("Vulnerability of numResults field in Scoreboard, in terms of tick");

 sbreturnCycleVul
				.name(name + ".sbVul.sbreturnCycleVul")
				.desc("Vulnerability of returnCycle field in Scoreboard, in terms of tick");

sbwritingInstVul
				.name(name + ".sbVul.sbwritingInstVul")
				.desc("Vulnerability of writingInst field in Scoreboard, in terms of tick");

sbnumUnpredResultsVul
				.name(name + ".sbVul.sbnumUnpredResultsVul")
				.desc("Vulnerability of numUnpredResults field in Scoreboard, in terms of tick");

sbfuIndicesVul
				.name(name + ".sbVul.sbfuIndicesVul")
				.desc("Vulnerability of fuIndices field in Scoreboard, in terms of tick");
/*
			sbVul
				.name(name + ".sbVul")
				.desc("Vulnerability of Scoreboard, in terms of tick");
*/
			registerFileVul
				.name(name + ".registerFileVul")
				.desc("Vulnerability of register file, in terms of tick");
			inFlightQueVul
				.name(name + ".inFlightQueVul")
				.desc("Vulnerability of inFlight instruction queue, in terms of tick");
			inFUMemQueVul
				.name(name + ".inFUMemQueVul")
				.desc("Vulnerability of inFUMem Instruction queue, in terms of tick");

			exeF1BrBuff
				.name(name + ".exeF1BrBuff")
				.desc("Vulnerability of execute/fetch1 branch buffer, in terms of tick");
			committQueVul
				.name(name + ".committQueVul")
				.desc("Vulnerability of committe queue, in terms of tick");
			nnnn
				.name(name + ".nnnn")
				.desc("number of instruction gets comimited");
			fuVul
				.name(name + ".fuVul")
				.desc("Vulnerability of FUs queues, in terms of tick");
			DuplicatableInstrIntOpLiveTime
				.name(name + ".DuplicatableInstrIntOpLiveTime")
				.desc("Lifetime of Int. registers for duplicatable Instructuons");
			nonDuplicatableInstrIntOpLiveTime
				.name(name + ".nonDuplicatableInstrIntOpLiveTime")
				.desc("Lifetime of Int. registers for nonduplicatable Instructuons");


			DupInstregisterFileVul
				.name(name + ".DupInstregisterFileVul")
				.desc("Register file vulnerability because of Duplicatable Instructions");
			nonDupInstregisterFileVul
				.name(name + ".nonDupInstregisterFileVul")
				.desc("Register file vulnerability because of nonDuplicatable Instructions");


			DupInstrVul
				.name(name + ".DupInstrVul")
				.desc("vulnerability comes from Duplicatable Instructions");

			nonDupInstrVul
				.name(name + ".nonDupInstrVul")
				.desc("vulnerability comes from nonDuplicatable Instructions");


numNonDuplicatableInsts
				.name(name + ".numNonDuplicatableInsts")
				.desc("number of non-duplicatable instructions committed");
numDuplicatableInsts
				.name(name + ".numDuplicatableInsts")
				.desc("number of duplicatable instructions committed");
numLoads
				.name(name + ".numLoads")
				.desc("number of LOAD instructions committed");
numStors
				.name(name + ".numStors")
				.desc("number of STORE instructions committed");
numCMP
				.name(name + ".numCMP")
				.desc("number of COMPARE instructions committed");
numBr
				.name(name + ".numBr")
				.desc("number of BRANCH instructions committed");
numSpeDest
				.name(name + ".numSpeDest")
				.desc("number of instructions with special regsiter as destination committed");

numForwardedLoad
                                .name(name + ".numForwardedLoad")
                                .desc("number of forwarded load from store queue.");



                       // LSQVul=lsqTransQueVul.value()+lsqReqsQueVul.value()+lsqStBufVul.value();


		}

};

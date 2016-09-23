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

/**
 * @file
 *
 *  Top level definition of the Minor in-order CPU model
 */

#ifndef __CPU_MINOR_CPU_HH__
#define __CPU_MINOR_CPU_HH__

#include "cpu/minor/activity.hh"
#include "cpu/minor/stats.hh"
#include "cpu/base.hh"
#include "cpu/simple_thread.hh"
#include "params/MinorCPU.hh"

//#include "base/vulnerability/vul_regfile.hh"                    //VUL_RF
#include "base/vulnerability/vulnerabilityParams.hh"            //VUL_PIPELINE
#include "base/vulnerability/vulnerabilityContainer.hh"         //VUL_PIPELINE
//#include  "SWIFT.hh"

/*
#ifndef __BASE_VULNERABILITY_VULREGFILE_HH__
#define __BASE_VULNERABILITY_VULREGFILE_HH__
*/

using namespace vulContext;                                     //VUL_PIPELINE

namespace Minor
{
/** Forward declared to break the cyclic inclusion dependencies between
 *  pipeline and cpu */
class Pipeline;

/** Minor will use the SimpleThread state for now */
typedef SimpleThread MinorThread;
};

/**
 *  MinorCPU is an in-order CPU model with four fixed pipeline stages:
 *
 *  Fetch1 - fetches lines from memory
 *  Fetch2 - decomposes lines into macro-op instructions
 *  Decode - decomposes macro-ops into micro-ops
 *  Execute - executes those micro-ops
 *
 *  This pipeline is carried in the MinorCPU::pipeline object.
 *  The exec_context interface is not carried by MinorCPU but by
 *      Minor::ExecContext objects
 *  created by Minor::Execute.
 */
class MinorCPU : public BaseCPU
{
  protected:
    /** Event for delayed wakeup of a thread */
    class ThreadActivateEvent : public Event
    {
      public:
        MinorCPU &cpu;
        ThreadID thread_id;

        ThreadActivateEvent(MinorCPU &cpu_, ThreadID thread_id_) :
            cpu(cpu_), thread_id(thread_id_)
        { }

        void process();
    };

    /** Events to wakeup each thread */
    std::vector<ThreadActivateEvent *> threadActivateEvents;

    /** pipeline is a container for the clockable pipeline stage objects.
     *  Elements of pipeline call TheISA to implement the model. */
    Minor::Pipeline *pipeline;

  public:
    /** Activity recording for pipeline.  This belongs to Pipeline but
     *  stages will access it through the CPU as the MinorCPU object
     *  actually mediates idling behaviour */
    Minor::MinorActivityRecorder *activityRecorder;

    /** These are thread state-representing objects for this CPU.  If
     *  you need a ThreadContext for *any* reason, use
     *  threads[threadId]->getTC() */
    std::vector<Minor::MinorThread *> threads;




  public:
    /** Provide a non-protected base class for Minor's Ports as derived
     *  classes are created by Fetch1 and Execute */
    class MinorCPUPort : public MasterPort
    {
      public:
        /** The enclosing cpu */
        MinorCPU &cpu;

      public:
        MinorCPUPort(const std::string& name_, MinorCPU &cpu_)
            : MasterPort(name_, &cpu_), cpu(cpu_)
        { }

      protected:
        /** Snooping a coherence request, do nothing.  */
        virtual void recvTimingSnoopReq(PacketPtr pkt) { }
    };

    /** The DrainManager passed into drain that needs be signalled when
     *  draining is complete */
    DrainManager *drainManager;

  protected:
     /** Return a reference to the data port. */
    MasterPort &getDataPort();

    /** Return a reference to the instruction port. */
    MasterPort &getInstPort();

  public:
    MinorCPU(MinorCPUParams *params);

    ~MinorCPU();

  public:
    /** Starting, waking and initialisation */
    void init();
    void startup();
    void wakeup();

    Addr dbg_vtophys(Addr addr);

    /** Processor-specific statistics */
    Minor::MinorStats stats;

    /** Stats interface from SimObject (by way of BaseCPU) */
    void regStats();

    /** Simple inst count interface from BaseCPU */
    Counter totalInsts() const;
    Counter totalOps() const;

    void serializeThread(std::ostream &os, ThreadID thread_id);
    void unserializeThread(Checkpoint *cp, const std::string &section,
        ThreadID thread_id);

    /** Serialize pipeline data */
    void serialize(std::ostream &os);
    void unserialize(Checkpoint *cp, const std::string &section);

    /** Drain interface */
    unsigned int drain(DrainManager *drain_manager);
    void drainResume();
    /** Signal from Pipeline that MinorCPU should signal the DrainManager
     *  that a drain is complete and set its drainState */
    void signalDrainDone();
    void memWriteback();

    /** Switching interface from BaseCPU */
    void switchOut();
    void takeOverFrom(BaseCPU *old_cpu);

    /** Thread activation interface from BaseCPU. */
    void activateContext(ThreadID thread_id, Cycles delay);
    void suspendContext(ThreadID thread_id);

    /** Interface for stages to signal that they have become active after
     *  a callback or eventq event where the pipeline itself may have
     *  already been idled.  The stage argument should be from the
     *  enumeration Pipeline::StageId */
    void wakeupOnEvent(unsigned int stage_id);
};
#endif /* __CPU_MINOR_EXECUTE_HH__ */
//ndif /* __CPU_MINOR_CPU_HH__ */
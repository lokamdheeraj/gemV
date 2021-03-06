/*
 * Copyright (c) 2011-2014 ARM Limited
 * Copyright (c) 2013 Advanced Micro Devices, Inc.
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
 * Copyright (c) 2002-2005 The Regents of The University of Michigan
 * All rights reserved.
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
 * Authors: Steve Reinhardt
 *          Dave Greene
 *          Nathan Binkert
 *          Andrew Bardsley
 */

/**
 * @file
 *
 *  ExecContext bears the exec_context interface for Minor.
 */

#ifndef __CPU_MINOR_EXEC_CONTEXT_HH__
#define __CPU_MINOR_EXEC_CONTEXT_HH__

#ifndef __BASE_VULNERABILITY_VULREGFILE_HH__
#define __BASE_VULNERABILITY_VULREGFILE__

#include "cpu/minor/execute.hh"
#include "cpu/minor/pipeline.hh"
#include "cpu/base.hh"
#include "cpu/simple_thread.hh"
#include "debug/MinorExecute.hh"
#include "debug/regFileVUL.hh"
#include "debug/regLiveNess.hh"
#include <string.h>
#include <vector>
#include <iomanip>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>
//#include  "base/vulnerability/vul_regfile.hh"


namespace Minor
{

/* Forward declaration of Execute */
class Execute;

/** ExecContext bears the exec_context interface for Minor.  This nicely
 *  separates that interface from other classes such as Pipeline, MinorCPU
 *  and DynMinorInst and makes it easier to see what state is accessed by it.
 */
class ExecContext
{
  public:
    MinorCPU &cpu;
    
    /** ThreadState object, provides all the architectural state. */
    SimpleThread &thread;

    /** The execute stage so we can peek at its contents. */
    Execute &execute;

    /** Instruction for the benefit of memory operations and for PC */
    MinorDynInstPtr inst;
RegLiveNess &regLiveNess;
////////registerfile vulnerability
  RegVulCalc  &regFileVul;
//Execute::RegLiveNess &regLiveNess;
//Execute::RegLiveNess *regLiveNess;

    ExecContext (
        MinorCPU &cpu_,
        SimpleThread &thread_, Execute &execute_,
        MinorDynInstPtr inst_, RegLiveNess &regLiveNess_, RegVulCalc  &regFileVul_):
        cpu(cpu_),
        thread(thread_),
        execute(execute_),
        inst(inst_),
	regLiveNess(regLiveNess_),
        regFileVul(regFileVul_)
    {
        DPRINTF(MinorExecute, "ExecContext setting PC: %s\n", inst->pc);
        pcState(inst->pc);
        setPredicate(true);
//regLiveNess(Execute::RegLiveNess::RegLiveNess());
//regLiveNess();
        thread.setIntReg(TheISA::ZeroReg, 0);
#if THE_ISA == ALPHA_ISA
        thread.setFloatReg(TheISA::ZeroReg, 0.0);
#endif
    }

    Fault
    readMem(Addr addr, uint8_t *data, unsigned int size,
        unsigned int flags)
    {
        execute.getLSQ().pushRequest(inst, true /* load */, data,
            size, addr, flags, NULL);
        return NoFault;
    }

    Fault
    writeMem(uint8_t *data, unsigned int size, Addr addr,
        unsigned int flags, uint64_t *res)
    {
        execute.getLSQ().pushRequest(inst, false /* store */, data,
            size, addr, flags, res);
        return NoFault;
    }



                        enum ARMRegs
                        {
                                R0 = 0,
                                R1 = 1,
                                R2 = 2,
                                R3 = 3,
                                R4 = 4,
                                R5 = 5,
                                R6 = 6,
                                R7 = 7,
                                R8 = 8,
                                R9 = 9,
                                R10 = 10,
                                R11 = 11,
                                R12 =12,
                                SP=13,
                                LR=14,
                                PC=15
                        };
/*
enum Access1 {
	READ1,
    	WRITE1
};

enum Access1 {
	READ1,
    	WRITE1
};
class RegLiveNess
{

  public:
	Access1 preOp;
	Tick lastRead;
	Tick lastWrite;
	RegLiveNess()
	  :preOp(WRITE1), lastRead(0), lastWrite(0)
	{ }
};


//RegLiveNess regLiveNess;
//std::vector<RegLiveNess> regLiveNess;
RegLiveNess regLiveNess[20];
*/

bool isProgramVisReg(const StaticInst *si, int idx)
{
/*
if (si->srcRegIdx(idx) == R0 || si->srcRegIdx(idx) == R1 || si->srcRegIdx(idx) == R2 || si->srcRegIdx(idx) == R3 || si->srcRegIdx(idx) == R4 || si->srcRegIdx(idx) == R5 || si->srcRegIdx(idx) == R6 || si->srcRegIdx(idx) == R7 || si->srcRegIdx(idx) == R8 || si->srcRegIdx(idx) == R9 || si->srcRegIdx(idx) == R10 || si->srcRegIdx(idx) == R11 || si->srcRegIdx(idx) == R12 || si->srcRegIdx(idx) == R13 || si->srcRegIdx(idx) == R14 || si->srcRegIdx(idx) == R15 ) return true;
*/
if (si->srcRegIdx(idx) < 16) return true;
return false;
}

bool isProgramVisRegDes (const StaticInst *si, int idx)
{
if (si->destRegIdx(idx) < 16) return  true;
return false;
}

uint64_t
readIntRegOperand(const StaticInst *si, int idx)
{
/////////////////////////////
	int temp=0;
	temp += regFileVul.vulOnRead(si->srcRegIdx(idx));
	if (isProgramVisReg(si,idx) )
	{
	inst->regFileVul += temp;
DPRINTF(regFileVUL, "Inst: %s reads register [%s], it addes %s to registerfile vulnerability.Reg Value=%s \n",*inst,si->srcRegIdx(idx),temp);
//////////////
	regLiveNess.onRead(si->srcRegIdx(idx));
//FAULT INJECTION HAS TO BE DONE HERE

	DPRINTF(regLiveNess, "Inst: %s reads register [%s].\n",*inst, si->srcRegIdx(idx) );
//regLiveNess[si->srcRegIdx(idx)].lastRead = curTick();
//regLiveNess[si->srcRegIdx(idx)].preOp = (Minor::Execute::Access1) READ1;
//////////////////////////

	}
////////////////////////

uint64_t regcon; 

regcon=thread.readIntReg(si->srcRegIdx(idx));
static int counter=0;
if(((execute.CLOCKtck - 500) <= curTick()) && (execute.CLOCKtck >= curTick()) && (si->srcRegIdx(idx) == execute.FaultSite) && counter == 0)
	{
		srand (time(NULL));
		int k=rand()%32;
		uint64_t inj=(uint64_t)pow(2,k);
		regcon= regcon ^ inj;
		++counter;
		return regcon;
	}
else	
return regcon;
}

    TheISA::FloatReg
    readFloatRegOperand(const StaticInst *si, int idx)
    {

/////////////////////////////
int temp=0;
temp += regFileVul.vulOnRead(si->srcRegIdx(idx));
if (isProgramVisReg(si,idx)) 
{
inst->regFileVul += temp;
DPRINTF(regFileVUL, "Inst: %s reads register [%s], it addes %s to register file vulnerability.\n",*inst,si->srcRegIdx(idx),temp);

regLiveNess.onRead(si->srcRegIdx(idx));
DPRINTF(regLiveNess, "Inst: %s reads register [%s].\n",*inst, si->srcRegIdx(idx) );
}
////////////////////////

        int reg_idx = si->srcRegIdx(idx) - TheISA::FP_Reg_Base;
        return thread.readFloatReg(reg_idx);
    }

    TheISA::FloatRegBits
    readFloatRegOperandBits(const StaticInst *si, int idx)
    {

/////////////////////////////
int temp=0;
temp += regFileVul.vulOnRead(si->srcRegIdx(idx));
if (isProgramVisReg(si,idx))
{
 inst->regFileVul += temp;
DPRINTF(regFileVUL, "Inst: %s reads register [%s], it addes %s to register file vulnerability.\n",*inst,si->srcRegIdx(idx),temp);

regLiveNess.onRead(si->srcRegIdx(idx));
DPRINTF(regLiveNess, "Inst: %s reads register [%s].\n",*inst, si->srcRegIdx(idx) );
}
////////////////////////

        int reg_idx = si->srcRegIdx(idx) - TheISA::FP_Reg_Base;
        return thread.readFloatRegBits(reg_idx);
    }

    void
    setIntRegOperand(const StaticInst *si, int idx, uint64_t val)
    {
//std::cout << si->disassemble(0) << *inst << "\nreading register------exec_context.hh-----\n";

/////////////
DPRINTF(regFileVUL, "Inst: %s sets register [%s].\n",*inst,si->destRegIdx(idx));
regFileVul.vulOnWrite(si->destRegIdx(idx));
long readTime=0;
long temp = regLiveNess.onWrite(si->destRegIdx(idx), readTime);
if (/*isProgramVisRegDes(si,idx) && */temp)
{

		DPRINTF(regLiveNess, "Inst: %s sets register [%s]. this was alive for %s last Read @ %s.\n",*inst,si->destRegIdx(idx), temp, readTime);


inst->intRegLiveNess += temp;

}




//////////////

        thread.setIntReg(si->destRegIdx(idx), val);
    }

    void
    setFloatRegOperand(const StaticInst *si, int idx,
        TheISA::FloatReg val)
    {

/////////////
DPRINTF(regFileVUL, "Inst: %s sets register [%s].\n",*inst,si->destRegIdx(idx));
regFileVul.vulOnWrite(si->destRegIdx(idx));


long readTime=0;
long temp = regLiveNess.onWrite(si->destRegIdx(idx), readTime);
if (/*isProgramVisRegDes(si,idx) &&*/ temp)
{

		DPRINTF(regLiveNess, "Inst: %s sets register [%s]. this was alive for %s last Read @ %s.\n",*inst,si->destRegIdx(idx), temp, readTime);


inst->intRegLiveNess += temp;

}

//////////////

        int reg_idx = si->destRegIdx(idx) - TheISA::FP_Reg_Base;
        thread.setFloatReg(reg_idx, val);
    }

    void
    setFloatRegOperandBits(const StaticInst *si, int idx,
        TheISA::FloatRegBits val)
    {

/////////////
DPRINTF(regFileVUL, "Inst: %s sets register [%s].\n",*inst,si->destRegIdx(idx));
regFileVul.vulOnWrite(si->destRegIdx(idx));


long readTime=0;
long temp = regLiveNess.onWrite(si->destRegIdx(idx), readTime);
if (/*isProgramVisRegDes(si,idx) &&*/ temp)
{

		DPRINTF(regLiveNess, "Inst: %s sets register [%s]. this was alive for %s last Read @ %s.\n",*inst,si->destRegIdx(idx), temp, readTime);


inst->intRegLiveNess += temp;

}

//////////////

        int reg_idx = si->destRegIdx(idx) - TheISA::FP_Reg_Base;
        thread.setFloatRegBits(reg_idx, val);
    }

    bool
    readPredicate()
    {
        return thread.readPredicate();
    }

    void
    setPredicate(bool val)
    {
        thread.setPredicate(val);
    }

    TheISA::PCState
    pcState()
    {
        return thread.pcState();
    }

    void
    pcState(const TheISA::PCState &val)
    {
        thread.pcState(val);
    }

    TheISA::MiscReg
    readMiscRegNoEffect(int misc_reg)
    {
        return thread.readMiscRegNoEffect(misc_reg);
    }

    TheISA::MiscReg
    readMiscReg(int misc_reg)
    {
        return thread.readMiscReg(misc_reg);
    }

    void
    setMiscReg(int misc_reg, const TheISA::MiscReg &val)
    {
        thread.setMiscReg(misc_reg, val);
    }

    TheISA::MiscReg
    readMiscRegOperand(const StaticInst *si, int idx)
    {

/////////////////////////////
int temp=0;
temp += regFileVul.vulOnRead(si->srcRegIdx(idx));
if (isProgramVisReg(si,idx))
{
 inst->regFileVul += temp;
DPRINTF(regFileVUL, "Inst: %s reads register [%s], it addes %s to register file vulnerability.\n",*inst,si->srcRegIdx(idx),temp);

regLiveNess.onRead(si->srcRegIdx(idx));
DPRINTF(regLiveNess, "Inst: %s reads register [%s].\n",*inst, si->srcRegIdx(idx) );
}
////////////////////////

        int reg_idx = si->srcRegIdx(idx) - TheISA::Misc_Reg_Base;
        return thread.readMiscReg(reg_idx);
    }

    void
    setMiscRegOperand(const StaticInst *si, int idx,
        const TheISA::MiscReg &val)
    {

/////////////
DPRINTF(regFileVUL, "Inst: %s sets register [%s].\n",*inst,si->destRegIdx(idx));
regFileVul.vulOnWrite(si->destRegIdx(idx));

long readTime=0;
long temp = regLiveNess.onWrite(si->destRegIdx(idx), readTime);
if (/*isProgramVisRegDes(si,idx) &&*/ temp)
{

		DPRINTF(regLiveNess, "Inst: %s sets register [%s]. this was alive for %s last Read @ %s.\n",*inst,si->destRegIdx(idx), temp, readTime);


inst->intRegLiveNess += temp;

}

//////////////

        int reg_idx = si->destRegIdx(idx) - TheISA::Misc_Reg_Base;
        return thread.setMiscReg(reg_idx, val);
    }

    Fault
    hwrei()
    {
#if THE_ISA == ALPHA_ISA
        return thread.hwrei();
#else
        return NoFault;
#endif
    }

    bool
    simPalCheck(int palFunc)
    {
#if THE_ISA == ALPHA_ISA
        return thread.simPalCheck(palFunc);
#else
        return false;
#endif
    }

    void
    syscall(int64_t callnum)
    {
        if (FullSystem)
            panic("Syscall emulation isn't available in FS mode.\n");

        thread.syscall(callnum);
    }

    ThreadContext *tcBase() { return thread.getTC(); }

    /* @todo, should make stCondFailures persistent somewhere */
    unsigned int readStCondFailures() { return 0; }
    unsigned int
    setStCondFailures(unsigned int st_cond_failures)
    {
        return 0;
    }

    int contextId() { return thread.contextId(); }
    /* ISA-specific (or at least currently ISA singleton) functions */

    /* X86: TLB twiddling */
    void
    demapPage(Addr vaddr, uint64_t asn)
    {
        thread.getITBPtr()->demapPage(vaddr, asn);
        thread.getDTBPtr()->demapPage(vaddr, asn);
    }

    TheISA::CCReg
    readCCRegOperand(const StaticInst *si, int idx)
    {

/////////////////////////////
int temp=0;
temp += regFileVul.vulOnRead(si->srcRegIdx(idx));
if (isProgramVisReg(si,idx)) 
{
inst->regFileVul = temp;
DPRINTF(regFileVUL, "Inst: %s reads register [%s], it addes %s to register file vulnerability.\n",*inst,si->srcRegIdx(idx),temp);

regLiveNess.onRead(si->srcRegIdx(idx));
DPRINTF(regLiveNess, "Inst: %s reads register [%s].\n",*inst, si->srcRegIdx(idx) );
}
///////////////////////

        int reg_idx = si->srcRegIdx(idx) - TheISA::CC_Reg_Base;
        return thread.readCCReg(reg_idx);
    }

    void
    setCCRegOperand(const StaticInst *si, int idx, TheISA::CCReg val)
    {

/////////////
DPRINTF(regFileVUL, "Inst: %s sets register [%s].\n",*inst,si->destRegIdx(idx));
regFileVul.vulOnWrite(si->destRegIdx(idx));

long readTime=0;
long temp = regLiveNess.onWrite(si->destRegIdx(idx), readTime);
if (/*isProgramVisRegDes(si,idx) && */ temp)
{

		DPRINTF(regLiveNess, "Inst: %s sets register [%s]. this was alive for %s last Read @ %s.\n",*inst,si->destRegIdx(idx), temp, readTime);


inst->intRegLiveNess += temp;

}

//////////////

        int reg_idx = si->destRegIdx(idx) - TheISA::CC_Reg_Base;
        thread.setCCReg(reg_idx, val);
    }

    void
    demapInstPage(Addr vaddr, uint64_t asn)
    {
        thread.getITBPtr()->demapPage(vaddr, asn);
    }

    void
    demapDataPage(Addr vaddr, uint64_t asn)
    {
        thread.getDTBPtr()->demapPage(vaddr, asn);
    }

    /* ALPHA/POWER: Effective address storage */
    void setEA(Addr &ea)
    {
        inst->ea = ea;
    }

    BaseCPU *getCpuPtr() { return &cpu; }

    /* POWER: Effective address storage */
    Addr getEA()
    {
        return inst->ea;
    }

    /* MIPS: other thread register reading/writing */
    uint64_t
    readRegOtherThread(unsigned idx, ThreadID tid = InvalidThreadID)
    {
        SimpleThread *other_thread = (tid == InvalidThreadID
            ? &thread : cpu.threads[tid]);

        if (idx < TheISA::FP_Reg_Base) { /* Integer */
            return other_thread->readIntReg(idx);
        } else if (idx < TheISA::Misc_Reg_Base) { /* Float */
            return other_thread->readFloatRegBits(idx
                - TheISA::FP_Reg_Base);
        } else { /* Misc */
            return other_thread->readMiscReg(idx
                - TheISA::Misc_Reg_Base);
        }
    }

    void
    setRegOtherThread(unsigned idx, const TheISA::MiscReg &val,
        ThreadID tid = InvalidThreadID)
    {
        SimpleThread *other_thread = (tid == InvalidThreadID
            ? &thread : cpu.threads[tid]);

        if (idx < TheISA::FP_Reg_Base) { /* Integer */
            return other_thread->setIntReg(idx, val);
        } else if (idx < TheISA::Misc_Reg_Base) { /* Float */
            return other_thread->setFloatRegBits(idx
                - TheISA::FP_Reg_Base, val);
        } else { /* Misc */
            return other_thread->setMiscReg(idx
                - TheISA::Misc_Reg_Base, val);
        }
    }
};

}

#endif /* __CPU_MINOR_EXEC_CONTEXT_HH__ */
#endif /* ____ */

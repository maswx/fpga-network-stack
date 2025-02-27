/*
 * Copyright (c) 2019, Systems Group, ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include <stdint.h>
#include "../fns_config.hpp"

const uint16_t MAX_QPS = ROCE_STACK_MAX_QPS;

const uint16_t PMTU = 1408; //dividable by 8, 16, 32, 64
const uint16_t PMTU_WORDS = PMTU / (DATA_WIDTH/8);

static const uint32_t PCIE_BATCH_PKG = 12;
static const uint32_t PCIE_BATCH_SIZE = PMTU * PCIE_BATCH_PKG;



#define IP_VERSION 4

#include "../axi_utils.hpp"
#include "../ipv4/ipv4.hpp"
#include "../ipv6/ipv6.hpp"
#include "../udp/udp.hpp"
#include "../ib_transport_protocol/ib_transport_protocol.hpp"

#define DISABLE_CRC_CHECK

#if IP_VERSION == 6
typedef ipv6Meta ipMeta;
#else
typedef ipv4Meta ipMeta;
#endif

template <int WIDTH>
void rocev2_core(
			hls::stream<net_axis<WIDTH> >& s_axis_rx_data,
			hls::stream<net_axis<WIDTH> >& m_axis_tx_data,

			// RDMA
			hls::stream<txMeta>& s_axis_tx_meta,

			// RPC
			hls::stream<routedTxMeta>& m_axis_rx_rpc_params,

			//Memory
			hls::stream<routedMemCmd>& m_axis_mem_write_cmd,
			hls::stream<routedMemCmd>& m_axis_mem_read_cmd,
			hls::stream<net_axis<WIDTH> >& m_axis_mem_write_data,
			hls::stream<net_axis<WIDTH> >& s_axis_mem_read_data,
			
			//Interface
			hls::stream<qpContext>&	s_axis_qp_interface,
			hls::stream<ifConnReq>&	s_axis_qp_conn_interface,
			ap_uint<128> local_ip_address,

			//Debug output
			ap_uint<32>& regCrcDropPkgCount,
			ap_uint<32>& regInvalidPsnDropCount);

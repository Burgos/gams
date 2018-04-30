/**
 * Copyright (c) 2015 Carnegie Mellon University. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following acknowledgments and disclaimers.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. The names "Carnegie Mellon University," "SEI" and/or "Software
 *    Engineering Institute" shall not be used to endorse or promote products
 *    derived from this software without prior written permission. For written
 *    permission, please contact permission@sei.cmu.edu.
 * 
 * 4. Products derived from this software may not be called "SEI" nor may "SEI"
 *    appear in their names without prior written permission of
 *    permission@sei.cmu.edu.
 * 
 * 5. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 * 
 *      This material is based upon work funded and supported by the Department
 *      of Defense under Contract No. FA8721-05-C-0003 with Carnegie Mellon
 *      University for the operation of the Software Engineering Institute, a
 *      federally funded research and development center. Any opinions,
 *      findings and conclusions or recommendations expressed in this material
 *      are those of the author(s) and do not necessarily reflect the views of
 *      the United States Department of Defense.
 * 
 *      NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING
 *      INSTITUTE MATERIAL IS FURNISHED ON AN "AS-IS" BASIS. CARNEGIE MELLON
 *      UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR
 *      IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF
 *      FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS
 *      OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES
 *      NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT,
 *      TRADEMARK, OR COPYRIGHT INFRINGEMENT.
 * 
 *      This material has been approved for public release and unlimited
 *      distribution.
 **/

/**
 * @file ReferenceFrame.cpp
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the base reference Frame class
 **/

#include <gams/pose/ReferenceFrameFwd.h>
#include <gams/pose/ReferenceFrame.h>
#include <gams/pose/CartesianFrame.h>
#include <gams/pose/GPSFrame.h>
#include <gams/pose/Linear.h>
#include <gams/pose/Angular.h>
#include <gams/pose/Quaternion.h>

#include <random>

using madara::knowledge::KnowledgeBase;
using madara::knowledge::KnowledgeRecord;
using madara::knowledge::ContextGuard;
using madara::knowledge::KnowledgeMap;
using madara::knowledge::containers::NativeDoubleVector;
using kmiter = KnowledgeMap::iterator;

namespace gams
{
  namespace pose
  {
    std::map<std::string, std::weak_ptr<ReferenceFrameIdentity>>
      ReferenceFrameIdentity::idents_;

    std::mutex ReferenceFrameIdentity::idents_lock_;

    std::shared_ptr<ReferenceFrameIdentity>
      ReferenceFrameIdentity::find(std::string id)
    {
      std::lock_guard<std::mutex> guard(idents_lock_);

      auto find = idents_.find(id);
      if (find != idents_.end()) {
        auto ret = find->second.lock();
        return ret;
      }
      return nullptr;
    }

    void ReferenceFrameIdentity::gc()
    {
      std::lock_guard<std::mutex> guard(idents_lock_);

      for (auto ident_iter = idents_.begin(); ident_iter != idents_.end();) {
        if (auto ident = ident_iter->second.lock()) {
          std::lock_guard<std::mutex> guard(ident->versions_lock_);

          for (auto ver_iter = ident->versions_.begin(); ver_iter != ident->versions_.end();) {
            if (ver_iter->second.expired()) {
              auto tmp = ver_iter;
              ++ver_iter;
              ident->versions_.erase(tmp);
            } else {
              ++ver_iter;
            }
          }
          ++ident_iter;
        } else {
          auto tmp = ident_iter;
          ++ident_iter;
          idents_.erase(tmp);
        }
      }
    }

    std::shared_ptr<ReferenceFrameIdentity>
      ReferenceFrameIdentity::lookup(std::string id)
    {
      std::lock_guard<std::mutex> guard(idents_lock_);

      auto find = idents_.find(id);
      if (find != idents_.end()) {
        auto ret = find->second.lock();
        if (ret) {
          return ret;
        }
      }
      auto val = std::make_shared<ReferenceFrameIdentity>(id);
      std::weak_ptr<ReferenceFrameIdentity> weak{val};
      if (find != idents_.end()) {
        find->second = std::move(weak);
      } else {
        idents_.insert(std::make_pair(std::move(id), std::move(weak)));
      }
      return val;
    }

    static std::string make_random_id(size_t len)
    {
      // Avoid letters/numbers easily confused with others
      static const char alphabet[] = "23456789CDFHJKMNPRSTWXY";

      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> dis(0, sizeof(alphabet) - 2);

      std::string ret;
      ret.reserve(len + 2);
      ret += "{";
      for (size_t i = 0; i < len; ++i) {
        ret += alphabet[dis(gen)];
      }
      ret += "}";
      return ret;
    }

    std::shared_ptr<ReferenceFrameIdentity>
      ReferenceFrameIdentity::make_guid()
    {
      std::string key;
      decltype(idents_)::iterator find;
      for (;;) {
        key = make_random_id(30); // Over 128 bits of randomness
        find = idents_.find(key);
        if (find != idents_.end()) {
          if (!find->second.expired()) {
            continue;
          }
        }
        break;
      }
      auto val = std::make_shared<ReferenceFrameIdentity>(key);
      std::weak_ptr<ReferenceFrameIdentity> weak{val};
      if (find != idents_.end()) {
        find->second = std::move(weak);
      } else {
        idents_.insert(std::make_pair(std::move(key), std::move(weak)));
      }
      return val;
    }

    void ReferenceFrameVersion::save_as(
            KnowledgeBase &kb,
            std::string key) const
    {
      key += ".";
      size_t pos = key.size();

      ContextGuard guard(kb);

      if (type() != Cartesian) {
        key += "type";
        //std::cerr << "saving " << name() << " frame: " << id() << std::endl;
        kb.set(key, name());
        key.resize(pos);
      }

      const ReferenceFrame &parent = origin_frame();
      if (parent.valid()) {
        key += "parent";
        kb.set(key, parent.id());
        key.resize(pos);
      }

      key += "origin";
      NativeDoubleVector vec(key, kb, 6);
      origin().to_container(vec);

      interpolated_ = false;
      ident().register_version(timestamp(),
          const_cast<ReferenceFrameVersion*>(this)->shared_from_this());
    }

    namespace {
      kmiter next_prefix(const KnowledgeMap &map, kmiter cur, const std::string &prefix)
      {
        while (cur != map.end() && cur->first.compare(0, prefix.size(), prefix) == 0) {
          ++cur;
        }
        return cur;
      }

      template<size_t N>
      std::array<const char *, N> split_string_array(std::string &s, char delim) {
        std::array<const char *, N> ret;
        size_t i = 0;
        char *cur = &s[0];

        while (i < N) {
          char *next = std::strchr(cur, delim);
          ret[i] = cur;
          if (next) {
            cur = next + 1;
            ++i;
          } else {
            ++i;
            break;
          }
        }

        while (i < N) {
          ret[i] = nullptr;
          ++i;
        }

        return ret;
      }

      std::pair<std::string, uint64_t> parse_frame_key(std::string key)
      {
        auto parts = split_string_array<2>(key, '.');
        size_t id_len = parts[1] - parts[0] - 1;

        uint64_t timestamp;
        if (std::strcmp(parts[1], "default") == 0) {
          timestamp = -1;
        } else {
          std::istringstream iss(parts[1]);
          iss >> std::hex >> timestamp;
        }

        key.resize(id_len);
        key.shrink_to_fit();
        return std::make_pair(key, timestamp);
      }

      std::pair<std::shared_ptr<ReferenceFrameVersion>, std::string>
        load_single( KnowledgeBase &kb, const std::string &id,
            uint64_t timestamp)
      {
        auto ident = ReferenceFrameIdentity::find(id);
        if (ident) {
          auto ver = ident->get_version(timestamp);
          if (ver) {
            //std::cerr << id << " already loaded" << std::endl;
            return std::make_pair(std::move(ver), std::string());
          }
        }

        auto key = impl::make_kb_prefix();
        impl::make_kb_key(key, id, timestamp);

        key += ".";
        size_t pos = key.size();

        ContextGuard guard(kb);
        KnowledgeMap &map = kb.get_context().get_map_unsafe();

        std::string parent_name;

        key += "parent";
        auto find = map.find(key);
        if (find != map.end()) {
          parent_name = find->second.to_string();
        }
        key.resize(pos);

        const ReferenceFrameType *type = Cartesian;
        key += "type";
        find = map.find(key);
        //std::cerr << "Looking for " << key << std::endl;
        if (find != map.end()) {
          std::string type_name = find->second.to_string();
          //std::cerr << "loaded " << type_name << " frame: " << id << std::endl;
          if (type_name == "GPS") {
            type = GPS;
          }
        }
        key.resize(pos);

        Pose origin(ReferenceFrame{});
        key += "origin";
        find = map.find(key);
        if (find != map.end()) {
          NativeDoubleVector vec(key, kb, 6);
          origin.from_container(vec);
        } else {
          return std::make_pair(std::shared_ptr<ReferenceFrameVersion>(),
              std::string());
        }

        auto ret = std::make_shared<ReferenceFrameVersion>(
            type, id, std::move(origin), timestamp);

        //std::cerr << "Made new " << id << " at " << (void*)ret.get() << std::endl;

        return std::make_pair(std::move(ret), std::move(parent_name));
      }
    }

    ReferenceFrame ReferenceFrameVersion::load_exact(
          KnowledgeBase &kb,
          const std::string &id,
          uint64_t timestamp)
    {
      ContextGuard guard(kb);

      auto ret = load_single(kb, id, timestamp);
      if (!ret.first) {
        return ReferenceFrame();
      }
      if (ret.second.size() > 0) {
        auto parent = load(kb, ret.second, timestamp);
        if (!parent.valid()) {
          //std::cerr << "Couldn't find " << ret.second << std::endl;
          return ReferenceFrame();
        }
        ret.first->mut_origin().frame(parent);
      }

      ret.first->ident().register_version(timestamp, ret.first);
      return ReferenceFrame(ret.first);
    }

    namespace {
      bool match_affixes(const std::string s,
          const char *prefix, size_t prefix_size, const std::string &suffix)
      {
        return (s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0 &&
                s.compare(0, prefix_size, prefix, prefix_size) == 0);
      }

      kmiter find_prev(kmiter i, kmiter begin,
          const char *prefix, size_t prefix_size, const std::string &suffix)
      {
        while (i != begin) {
          --i;
          const std::string &key = i->first;
          if (match_affixes(key, prefix, prefix_size, suffix)) {
            break;
          }
        }
        return i;
      }

      uint64_t timestamp_from_key(const char *key)
      {
        char *end;
        uint64_t ret = strtoull(key, &end, 16);
        if (key == end) {
          return -1;
        }
        return ret;
      }

      std::pair<uint64_t, uint64_t> find_nearest_neighbors(
          KnowledgeBase &kb, const std::string &id, uint64_t timestamp)
      {
        static const char suffix[] = ".origin";

        auto key = impl::make_kb_prefix();

        impl::make_kb_key(key, id);

        size_t len = key.size();

        impl::make_kb_key(key, timestamp);

        key += suffix;

        ContextGuard guard(kb);
        KnowledgeMap &map = kb.get_context().get_map_unsafe();

        auto next = map.lower_bound(key);

        if (next->first == key) {
          return std::make_pair(timestamp, timestamp);
        }

        auto prev = find_prev(next, map.begin(), key.c_str(), len, suffix);

        if (prev == next || prev->first.size() <= len + 1 ||
                            next->first.size() <= len + 1) {
          return std::make_pair(-1, -1);
        }

        uint64_t next_time;
        if (!match_affixes(next->first, key.c_str(), len, suffix)) {
          next_time = -1;
        } else {
          next_time = timestamp_from_key(&next->first[len + 1]);
        }

        uint64_t prev_time;
        if (!match_affixes(prev->first, key.c_str(), len, suffix)) {
          prev_time = -1;
        } else {
          prev_time = timestamp_from_key(&prev->first[len + 1]);
        }

        return std::make_pair(prev_time, next_time);
      }
    }

    uint64_t ReferenceFrameVersion::latest_timestamp(
            madara::knowledge::KnowledgeBase &kb,
            const std::string &id)
    {
      ContextGuard guard(kb);

      auto ret = find_nearest_neighbors(kb, id, -1).first;
      //std::cerr << "Latest for " << id << " is " << ret << std::endl;

      auto key = impl::make_kb_prefix();
      impl::make_kb_key(key, id, ret);
      key += ".parent";
      KnowledgeMap &map = kb.get_context().get_map_unsafe();
      auto find = map.find(key);
      if (find != map.end()) {
        auto p = latest_timestamp(kb, find->second.to_string());
        if (p < ret) {
          return ret;
        }
      }
      return ret;
    }

    ReferenceFrame ReferenceFrameVersion::load(
            KnowledgeBase &kb,
            const std::string &id,
            uint64_t timestamp)
    {
      ContextGuard guard(kb);

      if (timestamp == -1) {
        return load_exact(kb, id, timestamp);
      }

      ReferenceFrame ret = load_exact(kb, id, timestamp);
      if (ret.valid()) {
        return ret;
      }

      ret = load_exact(kb, id, -1);
      if (ret.valid()) {
        return ret;
      }

      auto pair = find_nearest_neighbors(kb, id, timestamp);

      //std::cerr << "Nearest " << id << " " << pair.first << " " << timestamp << " " << pair.second << std::endl;

      if (pair.first == -1 || pair.second == -1) {
        return {};
      }

      ReferenceFrame prev = load_exact(kb, id, pair.first);
      ReferenceFrame next = load_exact(kb, id, pair.second);

      ReferenceFrame parent;

      if (prev.origin_frame().valid() && next.origin_frame().valid()) {
        const std::string &parent_id = prev.origin_frame().id();
        const std::string &next_parent_id = next.origin_frame().id();

        if (parent_id != next_parent_id) {
          //std::cerr << "Mismatched frame parents " << parent_id << "  " << next_parent_id << std::endl;
          return {};
        }

        parent = load_exact(kb, parent_id, timestamp);
      }

      if (prev.valid() && next.valid()) {
        return prev.interpolate(next, std::move(parent), timestamp);
      }

      //std::cerr << "No interpolation found for " << id << std::endl;
      return {};
    }

    // TODO implement O(height) algo instead of O(height ^ 2)
    const ReferenceFrame *find_common_frame(
      const ReferenceFrame *from, const ReferenceFrame *to,
      std::vector<const ReferenceFrame *> *to_stack)
    {
      const ReferenceFrame *cur_to = to;
      for(;;)
      {
        const ReferenceFrame *cur_from = from;
        for(;;)
        {
          if(*cur_to == *cur_from)
          {
            return cur_to;
          }
          cur_from = &cur_from->origin().frame();
          if(!cur_from->valid())
            break;
        }
        if(to_stack)
          to_stack->push_back(cur_to);
        cur_to = &cur_to->origin().frame();
        if(!cur_to->valid())
          break;
      }
      return nullptr;
    }

    namespace simple_rotate {
      void orient_linear_vec(
            double &x, double &y, double &z,
            double rx, double ry, double rz,
            bool reverse)
      {
        if(rx == 0 && ry == 0 && rz == 0)
          return;

        Quaternion locq(x, y, z, 0);
        Quaternion rotq(rx, ry, rz);

        if(reverse)
          rotq.conjugate();

        locq.orient_by(rotq);
        locq.to_linear_vector(x, y, z);
      }

      void transform_angular_to_origin(
                        const ReferenceFrameType *,
                        const ReferenceFrameType *,
                        double orx, double ory, double orz,
                        double &rx, double &ry, double &rz)
      {
        Quaternion in_quat(rx, ry, rz);
        Quaternion origin_quat(orx, ory, orz);
        in_quat *= origin_quat;
        in_quat.to_angular_vector(rx, ry, rz);
      }

      void transform_angular_from_origin(
                        const ReferenceFrameType *,
                        const ReferenceFrameType *,
                        double orx, double ory, double orz,
                        double &rx, double &ry, double &rz)
      {
        Quaternion in_quat(rx, ry, rz);
        Quaternion origin_quat(orx, ory, orz);
        origin_quat.conjugate();
        in_quat *= origin_quat;
        in_quat.to_angular_vector(rx, ry, rz);
      }

      void transform_pose_to_origin(
                      const ReferenceFrameType *other,
                      const ReferenceFrameType *self,
                      double ox, double oy, double oz,
                      double orx, double ory, double orz,
                      double &x, double &y, double &z,
                      double &rx, double &ry, double &rz)
      {
        self->transform_linear_to_origin(other, self,
                          ox, oy, oz,
                          orx, ory,
                          orz, x, y, z);
        self->transform_angular_to_origin(other, self,
                          orx, ory, orz,
                          rx, ry, rz);
      }

      void transform_pose_from_origin(
                      const ReferenceFrameType *other,
                      const ReferenceFrameType *self,
                      double ox, double oy, double oz,
                      double orx, double ory, double orz,
                      double &x, double &y, double &z,
                      double &rx, double &ry, double &rz)
      {
        self->transform_linear_from_origin(other, self,
                          ox, oy, oz,
                          orx, ory,
                          orz, x, y, z);
        self->transform_angular_from_origin(other, self,
                          orx, ory, orz,
                          rx, ry, rz);
      }

      double calc_angle(
                  const ReferenceFrameType *,
                  double rx1, double ry1, double rz1,
                  double rx2, double ry2, double rz2)
      {
        Quaternion quat1(rx1, ry1, rz1);
        Quaternion quat2(ry2, ry2, rz2);
        return quat1.angle_to(quat2);
      }
    }

    const ReferenceFrame &default_frame (void)
    {
      static ReferenceFrame frame{"default_frame", Pose(ReferenceFrame(), 0, 0)};
      return frame;
    }
  }
}

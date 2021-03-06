/**
 * Copyright (c) 2014 Carnegie Mellon University. All Rights Reserved.
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
 * @file AccentStatus.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the definition of the accent-prefixed MADARA variables
 **/

#ifndef   _GAMS_VARIABLES_ACCENTS_H_
#define   _GAMS_VARIABLES_ACCENTS_H_

#include <vector>
#include <string>

#include "gams/GAMSExport.h"
#include "madara/knowledge/containers/Integer.h"
#include "madara/knowledge/containers/Double.h"
#include "madara/knowledge/containers/String.h"
#include "madara/knowledge/containers/NativeDoubleVector.h"
#include "madara/knowledge/containers/Vector.h"
#include "madara/knowledge/KnowledgeBase.h"

namespace gams
{
  namespace variables
  {
    /**
     * A container for accent status information
     **/
    class GAMSExport AccentStatus
    {
    public:
      /**
       * Constructor
       **/
      AccentStatus ();

      /**
       * Destructor
       **/
      ~AccentStatus ();

      /**
       * Assignment operator
       * @param  accent   accent to copy
       **/
      void operator= (const AccentStatus & accent);

      /**
       * Initializes variable containers
       * @param   knowledge  the variable context
       * @param   prefix     the prefix of the accents (e.g. swarm/agent)
       **/
      void init_vars (madara::knowledge::KnowledgeBase & knowledge,
        const std::string & prefix);
      
      /**
       * Initializes variable containers
       * @param   knowledge  the variable context
       * @param   prefix     the prefix of the accents (e.g. swarm/agent)
       **/
      void init_vars (madara::knowledge::Variables & knowledge,
        const std::string & prefix);

      /// accent specific command
      madara::knowledge::containers::String command;

      /// number of arguments for command
      madara::knowledge::containers::Vector command_args;

    protected:
      /**
       * Set variable settings
       */
      void init_variable_settings ();
    };

    /**
     * An array of accents
     **/
    typedef std::vector <AccentStatus>   AccentStatuses;
    
    /**
      * Initializes accent status containers
      * @param   variables  the variables to initialize
      * @param   knowledge  the knowledge base that houses the variables
      * @param   prefix     the prefix of the accents (e.g. swarm/agent)
      **/
    GAMSExport void init_vars (AccentStatuses & variables,
      madara::knowledge::KnowledgeBase & knowledge,
      const std::string & prefix);
    
    /**
      * Initializes accent status containers
      * @param   variables  the variables to initialize
      * @param   knowledge  the knowledge base that houses the variables
      * @param   prefix     the prefix of the accents (e.g. swarm/agent)
      **/
    GAMSExport void init_vars (AccentStatuses & variables,
      madara::knowledge::Variables & knowledge,
      const std::string & prefix);
  }
}

#endif // _GAMS_VARIABLES_ACCENTS_H_

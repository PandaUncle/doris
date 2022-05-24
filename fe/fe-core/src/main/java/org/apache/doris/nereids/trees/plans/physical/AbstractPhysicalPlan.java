// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

package org.apache.doris.nereids.trees.plans.physical;

import org.apache.doris.nereids.operators.plans.physical.PhysicalOperator;
import org.apache.doris.nereids.properties.LogicalProperties;
import org.apache.doris.nereids.properties.PhysicalProperties;
import org.apache.doris.nereids.trees.NodeType;
import org.apache.doris.nereids.trees.expressions.Slot;
import org.apache.doris.nereids.trees.plans.AbstractPlan;
import org.apache.doris.nereids.trees.plans.Plan;

import java.util.List;
import java.util.Objects;

/**
 * Abstract class for all concrete physical plan.
 */
public abstract class AbstractPhysicalPlan<
            PLAN_TYPE extends AbstractPhysicalPlan<PLAN_TYPE, OP_TYPE>,
            OP_TYPE extends PhysicalOperator>
        extends AbstractPlan<PLAN_TYPE, OP_TYPE>
        implements PhysicalPlan<PLAN_TYPE, OP_TYPE> {

    protected final LogicalProperties logicalProperties;
    protected final PhysicalProperties physicalProperties;

    /**
     * create physical plan by op, logicalProperties and children.
     */
    public AbstractPhysicalPlan(NodeType type, OP_TYPE operator,
            LogicalProperties logicalProperties, Plan... children) {
        super(type, operator, children);
        this.logicalProperties = Objects.requireNonNull(logicalProperties, "logicalProperties can not be null");
        // TODO: compute physical properties
        this.physicalProperties = new PhysicalProperties();
    }

    @Override
    public List<Slot> getOutput() {
        return logicalProperties.getOutput();
    }

    @Override
    public LogicalProperties getLogicalProperties() {
        return logicalProperties;
    }

    public PhysicalProperties getPhysicalProperties() {
        return physicalProperties;
    }
}

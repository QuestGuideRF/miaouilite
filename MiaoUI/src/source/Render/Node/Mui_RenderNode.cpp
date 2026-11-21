
#include <Render/Node/Mui_RenderNode.h>
#include <utility>
namespace Mui::Render
{
	MRenderNode::MRenderNode(MRenderNode* parent)
	{
		M_ASSERT(parent)
		parent->AddChildNode(this);
	}
	MRenderNode::~MRenderNode()
	{
		if(m_root)
			m_root->UnbindNodeRenderFunc(this);
	}
	MRenderNode::MRenderNode(MNodeRoot* root)
	{
		m_root = root;
		m_root->BindNodeRenderFunc(nullptr, this);
	}
	void MRenderNode::DelChildNode(std::vector<MRenderNode*>::iterator& iter)
	{
		std::lock_guard lock(m_lock);
		m_root->UnbindNodeRenderFunc(*iter);
		m_nodeList.erase(iter);
	}
	void MRenderNode::Visible(bool visible)
	{
		m_visible = visible;
		visible = m_parentVisible && visible;
		std::function<void(MRenderNode*, bool)> setchild = [&setchild, this](MRenderNode* node, bool _visible)
		{
			node->m_parentVisible = _visible;
			for (auto& child : node->m_nodeList)
			{
				setchild(child, _visible && node->m_visible);
			}
		};
		for (auto& child : m_nodeList)
		{
			setchild(child, visible);
		}
	}
	bool MRenderNode::Visible() const
	{
		return m_visible && m_parentVisible;
	}
	void MRenderNode::Name(const UIString& name)
	{
		m_name = name;
	}
	UIString MRenderNode::Name() const
	{
		return m_name;
	}
	void MRenderNode::AddChildNode(MRenderNode* node)
	{
		M_ASSERT(node)
		std::lock_guard lock(m_lock);
		node->m_parent = this;
		node->m_render = m_render;
		node->m_root = m_root;
		node->m_parentVisible = Visible();
		auto last = this;
		std::function<void(MRenderNode*)> findlast = [&last, &findlast](MRenderNode* parent)
		{
			last = parent;
			if (!parent->m_nodeList.empty())
				findlast(parent->m_nodeList.back());
		};
		findlast(this);
		m_root->BindNodeRenderFunc(last, node);
		m_nodeList.push_back(node);
	}
	bool MRenderNode::DelChildNode(MRenderNode* node)
	{
		std::lock_guard lock(m_lock);
		std::function<MRenderNode* (MRenderNode*)> findparent
			= [this, &findparent, &node](MRenderNode* _node) -> MRenderNode*
		{
			for (auto& child : _node->m_nodeList)
			{
				if (child == node)
					return child->m_parent;
				auto&& ret = findparent(child);
				if (ret == nullptr)
					continue;
				return ret;
			}
			return nullptr;
		};
		if(node->m_parent != this)
		{
			if (auto parent = findparent(this))
				parent->DelChildNode(node);
			else
				return false;
		}
		for (auto iter = m_nodeList.begin(); iter != m_nodeList.end(); ++iter)
		{
			if ((*iter) == node)
			{
				DelChildNode(iter);
				node->m_parent = nullptr;
				node->m_render = nullptr;
				node->m_root = nullptr;
				return true;
			}
		}
		return false;
	}
	bool MRenderNode::DelChildNode(const UIString& name)
	{
		std::lock_guard lock(m_lock);
		for (auto iter = m_nodeList.begin(); iter != m_nodeList.end(); ++iter)
		{
			if ((*iter)->m_name == name)
			{
				DelChildNode(iter);
				return true;
			}
		}
		return false;
	}
	MRenderNode* MRenderNode::FindChildNode(const UIString& name) const
	{
		for (auto& node : m_nodeList)
		{
			if (node->m_name == name)
				return node;
			auto&& child = node->FindChildNode(name);
			if (child == nullptr)
				continue;
			return child;
		}
		return nullptr;
	}
	void MRenderNode::Parent(MRenderNode* node)
	{
		if (m_parent)
			m_parent->DelChildNode(node);
		node->AddChildNode(this);
	}
	MRenderNode* MRenderNode::Parent() const
	{
		return m_parent;
	}
	MNodeRoot::MNodeRoot(MRenderNode* root)
	{
		m_render = root->m_render;
		m_rootNode = root;
		root->m_root = this;
	}
	MNodeRoot::~MNodeRoot() = default;
	MRenderNode* MNodeRoot::RootNode() const
	{
		return m_rootNode;
	}
	void MNodeRoot::RenderTree(void* data)
	{
		std::lock_guard lock(mx);
		for (auto& node : m_drawList)
		{
			if (node.node->Visible())
			{
				node.node->OnRender(m_render, data);
				if (node.node->m_nodeList.empty())
				{
					node.node->OnRenderChildEnd(m_render, data);
				}
			}
			auto parent = node.node->m_parent;
			if (node.node != parent->m_nodeList.back())
				continue;
			if (!node.node->m_nodeList.empty())
				continue;
			while (parent)
			{
				if (parent->Visible())
					parent->OnRenderChildEnd(m_render, data);
				if (auto pp = parent->m_parent; pp && parent != pp->m_nodeList.back())
					break;
				parent = parent->m_parent;
			}
		}
	}
	void MNodeRoot::BindNodeRenderFunc(MRenderNode* last, MRenderNode* node)
	{
		std::lock_guard lock(mx);
		auto getDrawNode = [this](MRenderNode* _node)
		{
			MNodeRoot::drawNode draw;
			draw.node = _node;
			return draw;
		};
		std::vector<drawNode> insertList;
		auto insertPos = m_drawList.end();
		if (last)
		{
			for (auto iter = m_drawList.begin(); iter != m_drawList.end(); ++iter)
			{
				if ((*iter).node == last)
				{
					insertPos = iter + 1;
					break;
				}
			}
		}
		std::function<void(MRenderNode*)> pushList = [&insertList, &pushList, &getDrawNode](MRenderNode* _node)
		{
			for (auto& child : _node->m_nodeList)
			{
				insertList.push_back(getDrawNode(child));
				pushList(child);
			}
		};
		insertList.push_back(getDrawNode(node));
		pushList(node);
		m_drawList.insert(insertPos, insertList.begin(), insertList.end());
	}
	void MNodeRoot::UnbindNodeRenderFunc(MRenderNode* node)
	{
		std::lock_guard lock(mx);
		auto unbindfun = [this](MRenderNode* _node, _m_size begin)
		{
			for (auto _it = m_drawList.begin() + (_m_long64)begin; _it != m_drawList.end(); ++_it)
			{
				if ((*_it).node == _node)
				{
					_it = m_drawList.erase(_it);
					return _m_size(_it - m_drawList.begin());
				}
			}
			return _m_size(0);
		};
		_m_size beginIndex = unbindfun(node, 0);
		std::function<void(MRenderNode*, _m_size)> findChild = [&unbindfun, &findChild, this](MRenderNode* _node, _m_size begin)
		{
			for (auto& child : _node->m_nodeList)
			{
				begin = unbindfun(child, begin);
				if (m_callback)
					m_callback(child);
				findChild(child, begin);
			}
		};
		findChild(node, beginIndex);
	}
}
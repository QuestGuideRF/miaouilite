
#pragma once
#include <Render/Mui_RenderMgr.h>
namespace Mui::Render
{
	class MNodeRoot;
	class MRenderNode
	{
	public:
		virtual ~MRenderNode();
	protected:
		MRenderNode() = default;
		MRenderNode(MRenderNode* parent);
		void Visible(bool visible);
		[[nodiscard]] bool Visible() const;
		void Name(const UIString& name);
		[[nodiscard]] UIString Name() const;
		void AddChildNode(MRenderNode* node);
		bool DelChildNode(MRenderNode* node);
		bool DelChildNode(const UIString& name);
		[[nodiscard]] MRenderNode* FindChildNode(const UIString& name) const;
		void Parent(MRenderNode* node);
		[[nodiscard]] MRenderNode* Parent() const;
		virtual void OnRender(MRenderCmd* render, void* data) = 0;
		virtual void OnRenderChildEnd(MRenderCmd* render, void* data) = 0;
		MRenderNode(MNodeRoot* root);
		void DelChildNode(std::vector<MRenderNode*>::iterator& iter);
		auto& GetNodeList() { return m_nodeList; }
		MRenderCmd* m_render = nullptr;
	private:
		bool m_visible = true;
		bool m_parentVisible = true;
		MRenderNode* m_parent = nullptr;
		MNodeRoot* m_root = nullptr;
		UIString m_name;
		std::vector<MRenderNode*> m_nodeList;
		std::recursive_mutex m_lock;
		friend class MNodeRoot;
	};
	class MNodeRoot
	{
	public:
		MNodeRoot(MRenderNode* root);
		virtual ~MNodeRoot();
		[[nodiscard]] MRenderNode* RootNode() const;
		void RenderTree(void* data);
		[[nodiscard]] size_t GetCount() const
		{
			return m_drawList.size();
		}
		void SetUnBindCallback(std::function<void(MRenderNode*)>&& callback)
		{
			m_callback = std::move(callback);
		}
	private:
		std::mutex mx;
		struct drawNode
		{
			MRenderNode* node = nullptr;
		};
		std::vector<drawNode> m_drawList;
		MRenderNode* m_rootNode = nullptr;
		MRenderCmd* m_render = nullptr;
		std::function<void(MRenderNode*)> m_callback = nullptr;
		void BindNodeRenderFunc(MRenderNode* last, MRenderNode* node);
		void UnbindNodeRenderFunc(MRenderNode* node);
		friend class MRenderNode;
	};
}